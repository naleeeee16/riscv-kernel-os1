#include "../h/sem.hpp"
#include "../h/scheduler.hpp"
#include "../h/memoryallocator.hpp"
#include "../h/print.hpp"
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

Sem *Sem::semCreate(int init) {
    Sem* s= new Sem(init);
    RiscV::semaphores->addFirst(s);
    return s;
}

Sem *Sem::open(int init) {
    return semCreate(init);
}

int Sem::close() {
    if (closed == true)return -1;
    closed = true;
    while (!blocked->isEmpty()) {
        deblock();
    }
    return 0;
}

Sem::~Sem() {
    delete blocked;
}

void Sem::block() {
    TCB::running->setState(TCB::BLOCKED);
    blocked->put(TCB::running);
    TCB::dispatch();
}

void Sem::deblock() {
    TCB *t = blocked->get();
    if (t != nullptr) {
        t->setState(TCB::READY);
        Scheduler::instance()->put(t);
    }
}

int Sem::wait() {
    if (closed == true && !blocked->isEmpty())return -1;
    if (--val < 0) block();
    return 0;
}

int Sem::signal() {
    if (++val <= 0)deblock();
    return 0;
}

void *Sem::operator new(size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void *Sem::operator new[](size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void Sem::operator delete(void *p) noexcept {
    MemoryAllocator::instance().mem_free(p);
}

void Sem::operator delete[](void *p) noexcept {
    MemoryAllocator::instance().mem_free(p);
}

void Sem::printSem() {
    if (closed) {
        printStr("JESTE ");
    } else {
        printStr("NIJE ");
    }
    printStr("ZATVOREN val: ");printInteger(val);
    printStr("Blokirane: ");
    BlockedList::Blocked *curr = blocked->head;
    while (curr) {
        curr->thread->printThread();
        curr = curr->next;
    }
    printStr("\n");
}
