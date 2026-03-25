#ifndef _SLEEPINGSCHEDULER_HPP
#define _SLEEPINGSCHEDULER_HPP

#include "../h/tcb.hpp"

class SleepingScheduler {
private:
    TCB *head = nullptr;

    void wake();

    void remove(uint64 id);

    int put(time_t time_slice); // stavljam pozivajucu- running nit da spava;

    SleepingScheduler();

public:

    static void init();

    static bool initialized;
    static SleepingScheduler *scheduler;

    void printList();

    static SleepingScheduler *instance();

    friend class TCB;

    friend class Trap;

    SleepingScheduler &operator=(const SleepingScheduler &m) = delete;

    SleepingScheduler &operator=(SleepingScheduler &&m) = delete;

    SleepingScheduler(SleepingScheduler &&) = delete;

    SleepingScheduler(const SleepingScheduler &) = delete;

    ~SleepingScheduler() {};

};


#endif
