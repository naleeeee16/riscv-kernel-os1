#ifndef _TCB_HPP
#define _TCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/sem.hpp"

class TCB {
public:
    enum State {
        INITIALIZED, RUNNING, READY, SLEEPY, BLOCKED, TERMINATING
    };

    void *operator new(size_t n);

    void *operator new[](size_t n);

    void operator delete(void *p) noexcept;

    void operator delete[](void *p) noexcept;

    ~TCB();

    using Body = void (*)(void *);

    static TCB *createThread(Body body, void *args, void *stack);

    static TCB *running;

    void printThread();

    static uint64 ID;
private:
    TCB(TCB::Body body, void *args, void *stackk, uint64 timeSlice = DEFAULT_TIME_SLICE) :
            body(body), stack((uint64 *) stackk),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0  }),
                     timeSlice(timeSlice), arg(args), userThread(true), id(ID++) { waitForMe = Sem::open(0);}

    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack; // ko je rekao da treba char
    Context context;
    uint64 timeSlice;
    State state;
    void *arg;
    bool userThread;
    uint64 id;
    Sem *waitForMe;
    uint64 waitforId = -1;
    time_t sleepingTime;

    TCB *nextReady = nullptr;
    TCB *nextSleeping = nullptr;

    friend class Trap;
    friend class RiscV;
    friend class Scheduler;
    friend class Sem;
    friend class SleepingScheduler;

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static void putSync();

    static int exit();

    static void join(TCB *handle);

    static uint64 timeSliceCounter;

    void makeReady();

    void setState(State value) { state = value; }

    State getState() const { return state; }

    uint64 getTimeSlice() const { return timeSlice; }

    void setSleepingTime(time_t t) { sleepingTime = t; }

};

#endif
