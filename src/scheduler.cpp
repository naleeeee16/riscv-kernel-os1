#include "../h/scheduler.hpp"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"
/*[idle nit]->[nit1]->[nit2]->[nit3]
 *  get () ---> [idle nit]->[nit1]->[nit2]
 * put(nit3) ---> [idle nit]->[nit3]->[nit1]->[nit2]
*/
#include "../h/tcb.hpp"


bool Scheduler::initialized = false;
Scheduler *Scheduler::scheduler = nullptr;

TCB *Scheduler::get() {
    TCB *curr = readyThreadHead;
    TCB *prev = nullptr;
    while (curr->nextReady != nullptr) {
        prev = curr;
        curr = curr->nextReady;
    }
    if (prev != nullptr) {
        prev->nextReady = nullptr;
    }
    return curr;
}

void Scheduler::put(TCB *tcb) {
    if (readyThreadHead == nullptr) {
        readyThreadHead = tcb;
        return;
    }
    tcb->nextReady = readyThreadHead->nextReady;
    readyThreadHead->nextReady = tcb;

}

void Scheduler::remove(uint64 id) {
    TCB *curr = readyThreadHead;
    TCB *prev = nullptr;
    while (curr) {
        if (curr->id == id)break;
        prev = curr;
        curr = curr->nextReady;
    }
    if (curr && curr != readyThreadHead) {
        prev->nextReady = curr->nextReady;
        curr->nextReady = nullptr;
    }
}

Scheduler *Scheduler::instance() {
    if (!initialized) {
        init();
        initialized = true;
    }
    return scheduler;
}

void Scheduler::init() {
    initialized = true;
    scheduler = (Scheduler *) MemoryAllocator::instance().mem_alloc(sizeof(Scheduler));
    void *stack = MemoryAllocator::instance().mem_alloc(sizeof(uint64) * DEFAULT_STACK_SIZE);
    Scheduler::instance()->readyThreadHead = TCB::createThread(RiscV::idle, (uint64 *) (100), stack);
    Scheduler::instance()->readyThreadHead->setState(TCB::READY);
   // RiscV::threads->addFirst(Scheduler::instance()->readyThreadHead);

}

void Scheduler::printList() {
    TCB *curr = readyThreadHead;
    while (curr) {
        curr->printThread();
        curr = curr->nextReady;
    }
}
