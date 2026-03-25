#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/sleepingscheduler.hpp"

KeyboardBuffer* RiscV::inBuff= nullptr;
KeyboardBuffer* RiscV:: outBuff= nullptr;
bool RiscV::end= false;
TCB* RiscV::put= nullptr;
List<Sem>* RiscV::semaphores;
List<TCB>* RiscV::threads;

void RiscV::packArgumentsAndEcall(int num, uint64 a1, uint64 a2, uint64 a3, uint64 a4) {
    __asm__ volatile ("ecall");
}

void RiscV::idle(void* v) {
    // printStr("idle");
    while (1){ }
}


void RiscV::putBuff(void* arg){
    char data = *(char *) CONSOLE_STATUS;
    while(1) {
        while (data & CONSOLE_TX_STATUS_BIT) {
            char c = RiscV::outBuff->get();
            uint64 *regTX = (uint64 *) CONSOLE_TX_DATA;
            *regTX = c;
            data = *(char *) CONSOLE_STATUS;

            if(RiscV::outBuff->getCount()==0 && RiscV::end==true) {
                endProgram();
            }
        }
    }
}

void RiscV::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

 void RiscV::endProgram(){
     dealocateAll();
     *((unsigned  int*)0x100000)=0x5555;
 }
 void RiscV::dealocateAll(){
     while(semaphores->num!=0){
         Sem * s= (Sem*) semaphores->removeFirst();
         if(s!= nullptr ) {;
             delete s;
         }
     }
    while(threads->num!=0){
        TCB * t= (TCB*) threads->removeFirst();
        if(t!= nullptr ) {;
            delete t;
        }
    }
     delete outBuff;
     delete inBuff;
}

void RiscV::init() {
    uint64 volatile base= (uint64)(&Trap::tableTrap) | 1;
    __asm__ volatile ("csrw stvec, %[vector] ": :[vector] "r" (base ));

    semaphores= (List<Sem>*) mem_alloc(sizeof(List<Sem>));
    semaphores->num=0;

    threads= (List<TCB>*) mem_alloc(sizeof(List<TCB>));
    threads->num=0;

    inBuff= new KeyboardBuffer();
    outBuff= new KeyboardBuffer();

    Scheduler::init();
    SleepingScheduler::init();

    TCB* main;
    thread_create(&main, nullptr, nullptr);
    main->userThread= false;
    main->setState(TCB::RUNNING);
    TCB::running=main;

    thread_create(&put, putBuff, nullptr);
    put->userThread=false;
}
