#ifndef _SEM_HPP
#define _SEM_HPP

#include "../h/blockedlist.hpp"
#include "../lib/hw.h"

class Sem {
public:
    void printSem();

    ~Sem();

    int wait();

    int signal();

    int close();

    int value() { return val; };

    static Sem *open(int init);

    void *operator new(size_t n);

    void *operator new[](size_t n);

    void operator delete(void *p) noexcept;

    void operator delete[](void *p) noexcept;

    friend class KeyboardBuffer;

private:

    Sem(int initValue = 1) : val(initValue) { initialized = true; }

    static Sem *semCreate(int init);

    void block();

    void deblock();

    int val;
    bool closed = false;
    bool initialized = false;

    BlockedList *blocked = new BlockedList();
};

#endif