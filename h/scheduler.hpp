#ifndef _SCHEDULER_HPP
#define _SCHEDULER_HPP

#include "../lib/hw.h"

class TCB;

class Scheduler {
private:

    TCB *readyThreadHead;

    Scheduler();

    void remove(uint64 id);

    TCB *get();

    void put(TCB *tcb);

public:
    friend class TCB;

    friend class Sem;

    static void init();

    static bool initialized;

    static Scheduler *scheduler;

    static Scheduler *instance();

    void printList();

    Scheduler &operator=(const Scheduler &m) = delete;

    Scheduler &operator=(Scheduler &&m) = delete;

    Scheduler(Scheduler &&) = delete;

    Scheduler(const Scheduler &) = delete;

    ~Scheduler() {};

};


#endif
