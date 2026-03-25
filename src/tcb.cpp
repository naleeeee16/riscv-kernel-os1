#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/print.hpp"
#include "../h/sleepingscheduler.hpp"

TCB *TCB::running = nullptr;
uint64  TCB::ID = 1;
uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void *args, void *stack) {
    TCB *thread = new TCB(body, args, stack, DEFAULT_TIME_SLICE);
    RiscV::threads->addFirst(thread);
    if (body != RiscV::idle) {
        thread->makeReady();
    }
    return thread;
}

TCB::~TCB() {
    sem_close(waitForMe);
    Scheduler::instance()->remove(this->id);
    SleepingScheduler::instance()->remove(this->id);
    this->setState(TERMINATING);
    delete stack;
}

int TCB::exit() {
    if (running == nullptr)return -1;
    running->setState(TERMINATING);
    running->waitForMe->close();
    dispatch();
    return 0;
}

void TCB::dispatch() {
    TCB *old = running;
    if (!(old->getState() == TERMINATING) && old->getState() != BLOCKED && old->getState() != SLEEPY && old->id!= 1) {
        Scheduler::instance()->put(old);
    }
    running = Scheduler::instance()->get();
    running->setState(RUNNING);
    if (old != running || old->id == 1) {
        if (running->userThread) {
            RiscV::mc_sstatus(RiscV::SSTATUS_SPP);//DA SE VRATI U USER
            RiscV::mc_sip(RiscV::SIP_SSIP);
            RiscV::ms_sie(RiscV::SIP_SSIP);
            RiscV::ms_sie(RiscV::SIP_SEIP);
        } else {
            RiscV::ms_sstatus(RiscV::SSTATUS_SPP);//DA SE VRATI U supervisor
            RiscV::mc_sstatus(RiscV::SSTATUS_SIE);
            RiscV::mc_sstatus(RiscV::SSTATUS_SPIE);
        }
        TCB::contextSwitch(&old->context, &running->context);
    }
}
 void TCB::putSync(){
    TCB* old= running;
    running= RiscV::put;
    Scheduler::instance()->remove(running->id);
    running->setState(RUNNING);
    if(old!=running){Scheduler::instance()->put(old);
        RiscV::ms_sstatus(RiscV::SSTATUS_SPP);//DA SE VRATI U USER
        RiscV::mc_sstatus(RiscV::SSTATUS_SIE);
        RiscV::mc_sstatus(RiscV::SSTATUS_SPIE);
        TCB::contextSwitch(&old->context, &running->context);
    }
 }

void TCB::join(TCB *handle) {
    if (handle->getState() == BLOCKED && handle->waitforId == running->id) {
        //printStr("DEADLOCK");
        return;
    }
    if (handle->id != running->id) {
        running->waitforId = handle->id;
        handle->waitForMe->wait();
    }

}

void TCB::threadWrapper() {
    RiscV::popSppSpie();
    running->body(running->arg);
    thread_exit();
}

void *TCB::operator new(size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void *TCB::operator new[](size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void TCB::operator delete(void *p) noexcept {
    MemoryAllocator::instance().mem_free(p);
}

void TCB::operator delete[](void *p) noexcept {
    MemoryAllocator::instance().mem_free(p);
}

void TCB::printThread() {
    printStr(" NIT ");
    printInteger(this->id);
    printStr(" arg: ");
    printInteger((uint64) arg);
    printStr("  ---->");
}

void TCB::makeReady() {
    if (body != nullptr) {
        Scheduler::instance()->put(this);
        this->setState(READY);
    }
}
