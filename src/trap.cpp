#include "../h/trap.hpp"
#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/sleepingscheduler.hpp"
#include "../h/print.hpp"


void Trap::syscallTrap() {

    uint64 a0 = RiscV::r_a0();
    uint64 volatile a4;
    __asm__ volatile ("mv %0, a4" : "=r" (a4));
    uint64 code = a0;
    uint64 scause = RiscV::r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc = RiscV::r_sepc() + 4;
        uint64 volatile sstatus = RiscV::r_sstatus();

        if (code == 0x01) {
            size_t size = RiscV::r_a1() * MEM_BLOCK_SIZE;
            MemoryAllocator::instance().mem_alloc(size);
        } else if (code == 0x02) {
            void *addr = (void *) RiscV::r_a1();
            MemoryAllocator::instance().mem_free(addr);

        } else if (code == 0x11) {
            TCB::Body startRoutine = (TCB::Body) RiscV::r_a2();
            thread_t *handle = (thread_t *) RiscV::r_a1();
            void *stack = (void *) a4;
            void *args;
            __asm__ volatile ("mv %0, a3" : "=r" (args));
            volatile uint64 ret = 0;
            *handle = TCB::createThread(startRoutine, args, stack);
            if (handle == nullptr)ret = -1;
            RiscV::w_a0(ret);
        } else if (code == 0x12) {
            volatile uint64 ret = TCB::exit();
            RiscV::w_a0(ret);
        } else if (code == 0x13) {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        } else if (code == 0x14) {
            thread_t handle = (thread_t) RiscV::r_a1();
            TCB::join(handle);
        }
        else if (code == 0x21) {
            uint64 init = RiscV::r_a2();
            sem_t *handle = (sem_t *) RiscV::r_a1();
            volatile uint64 ret = 0;
            *handle = Sem::open(init);
            if (handle == nullptr) ret = -1;
            RiscV::w_a0(ret);
        } else if (code == 0x22) {
            sem_t id = (sem_t) RiscV::r_a1();
            volatile uint64 ret;
            if (id == nullptr)ret = -1;
            else ret = id->close();
            RiscV::w_a0(ret);
        } else if (code == 0x23) {
            sem_t id = (sem_t) RiscV::r_a1();
            volatile uint64 ret;
            if (id == nullptr)ret = -1;
            else ret = id->wait();
            RiscV::w_a0(ret);
        } else if (code == 0x24) {
            sem_t id = (sem_t) RiscV::r_a1();
            volatile uint64 ret;
            if (id == nullptr)ret = -1;
            else ret = id->signal();
            RiscV::w_a0(ret);
        } else if (code == 0x31) {
            time_t time = (time_t) RiscV::r_a1();
            volatile uint64 ret;
            SleepingScheduler::instance()->put(time);
            RiscV::w_a0(ret);
        } else if (code == 0x41) {
            if (RiscV::inBuff->getCount() <0) {
                RiscV::inBuff->itemAvailable->wait();
            }
            volatile char ret = RiscV::inBuff->get();
            RiscV::w_a0(ret);
        }
        if (code == 0x42) {
            char c = RiscV::r_a1();
            if (RiscV::outBuff->getCount() == RiscV::outBuff->KB_SIZE) {
                TCB::putSync();
            }
            RiscV::outBuff->put(c);
        }

        RiscV::w_sstatus(sstatus);
        RiscV::w_sepc(sepc);
    } else {
        uint64 volatile sepc = RiscV::r_sepc();
        uint64 volatile sstatus = RiscV::r_sstatus();
        printStr("GRESKA: scause: ");
        printInteger(scause);
        switch (scause) {
            case 2:
                printStr(" = ilegalna instrukcija \n");
                break;
            case 5:
                printStr(" = nedozvoljena adresa citanja \n");
                break;
            case 7:
                printStr(" = nedozvoljena adresa upisa \n");
                break;
            default:
                printStr(" ostale greske...\n");
        };
        printStr("sepc: ");
        printInteger(sepc);
        printStr("\nsstatus: ");
        printInteger(sstatus);
        printStr("\n");

        RiscV::end = true;
        TCB::join(RiscV::put);
    }
}

void Trap::timerTrap() {
    uint64 scause = RiscV::r_scause();

    if (scause == 0x8000000000000001UL) {
        RiscV::mc_sip(RiscV::SIP_SSIP);

        TCB::timeSliceCounter++;
        SleepingScheduler::instance()->wake();
        if (TCB::timeSliceCounter >= TCB::running->getTimeSlice()) {
            uint64 volatile sepc = RiscV::r_sepc();
            uint64 volatile sstatus = RiscV::r_sstatus();
            TCB::timeSliceCounter = 0;

            TCB::dispatch();

            RiscV::w_sstatus(sstatus);
            RiscV::w_sepc(sepc);
        }
    }
}

void Trap::consoleTrap() {

    uint64 scause = RiscV::r_scause();
    if (scause == 0x8000000000000009UL) {
        int irq = plic_claim();
        if (irq == CONSOLE_IRQ) {
            char data = *(char *) CONSOLE_STATUS;
            while (data & CONSOLE_RX_STATUS_BIT) {
                uint64 *regRX = (uint64 *) CONSOLE_RX_DATA;
                char c = *regRX;
                if(RiscV::inBuff->getCount()!=KeyboardBuffer::KB_SIZE){
                    RiscV::inBuff->put(c);
                }
                data = *(char *) CONSOLE_STATUS;
            }
            plic_complete(irq);
        }

    }
}


