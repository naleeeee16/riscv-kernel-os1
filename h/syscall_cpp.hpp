#ifndef _SYSCALL_CPP_HPP
#define _SYSCALL_CPP_HPP

#include "syscall_c.hpp"

void *operator new(size_t);

void operator delete(void *) noexcept; // da li je ovo ok

class Thread {
public:
    Thread(void(*body)(void *), void *arg);

    virtual ~Thread();

    int start();

    void join();

    static void dispatch();

    static int sleep(time_t);

    friend void wrappRun(void *);

protected:
    Thread();

    virtual void run() {}

private:
    thread_t myHandle;

    void (*body)(void *);

    void *arg;
};

class Semaphore {
public:

    Semaphore(unsigned init = 1);

    virtual ~Semaphore();

    int wait();

    int signal();

private:
    sem_t myHandle;
};

class PeriodicThread : public Thread {
public:
    void terminate();

protected:
    PeriodicThread(time_t period);

    virtual void periodicActivation() {}

    void run() override;

private:

    time_t period;
};

class Console {
public:
    static char getc();

    static void putc(char);

};


#endif
