#ifndef _BLOCKED_HPP
#define _BLOCKED_HPP

#include "../lib/hw.h"

class TCB;

class BlockedList {

public:

    struct Blocked {
        TCB *thread;
        Blocked *next;
    };

    void put(TCB *t);

    TCB *get();

    void freeList();

    bool isEmpty();

    void *operator new(size_t n);

    void *operator new[](size_t n);

    void operator delete(void *p) noexcept;

    void operator delete[](void *p) noexcept;

private:
    Blocked *head = nullptr;

    friend class Sem;
};


#endif
