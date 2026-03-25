#include "../h/sleepingscheduler.hpp"
#include "../h/memoryallocator.hpp"
#include "../h/tcb.hpp"

bool SleepingScheduler::initialized = false;
SleepingScheduler *SleepingScheduler::scheduler = nullptr;


SleepingScheduler *SleepingScheduler::instance() {
    if (!initialized) {
        init();
        initialized = true;
    }
    return scheduler;
}

void SleepingScheduler::init() {
    initialized = true;
    scheduler = (SleepingScheduler *) MemoryAllocator::instance().mem_alloc(sizeof(SleepingScheduler));
    scheduler->head = nullptr;
}

void SleepingScheduler::wake() {
    if (!head)
        return;
    TCB *prev;
    while (head) {
        if (head->sleepingTime != 1)break;
        head->sleepingTime = 0;
        prev = head;
        prev->makeReady();
        head = head->nextSleeping;
        prev->nextSleeping = nullptr;
    }

    TCB *curr = head;
    prev = nullptr;

    while (curr) {
        TCB *old;
        if (curr->sleepingTime == 1) {
            old = curr;
            prev->nextSleeping = curr->nextSleeping;
            old->makeReady();
            old->nextSleeping = nullptr;
            old->sleepingTime = 0;
        } else {
            curr->sleepingTime--;
            prev = curr;
        }
        curr = curr->nextSleeping;
    }
}

int SleepingScheduler::put(time_t time_slice) {
    if (TCB::running == nullptr || time_slice <= 0)return -1;
    TCB::running->setSleepingTime(time_slice);
    TCB::running->setState(TCB::SLEEPY);

    if (head == nullptr) {
        head = TCB::running;
        head->nextSleeping = nullptr;
    } else {
        TCB::running->nextSleeping = head;
        head = TCB::running;
    }
    TCB::dispatch();
    return 0;
}


void SleepingScheduler::remove(uint64 id) {
    TCB *curr = head;
    TCB *prev = nullptr;
    while (curr) {
        if (curr->id == id)break;
        prev = curr;
        curr = curr->nextSleeping;
    }
    if (curr) {
        if (prev) {
            prev->nextSleeping = curr->nextSleeping;
        } else {
            head = curr->nextSleeping;
        }
        curr->nextSleeping = nullptr;
    }
}

void SleepingScheduler::printList() {
    TCB *curr = head;
    while (curr) {
        curr->printThread();
        curr = curr->nextSleeping;
    }
}