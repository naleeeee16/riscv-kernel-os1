#ifndef _BUFFER_HPP
#define _BUFFER_HPP
#include "../h/sem.hpp"
#include "../h/syscall_c.hpp"
#include "../h/memoryallocator.hpp"

class KeyboardBuffer {
public:
    KeyboardBuffer() :  head(0), tail(0),count(0) {
        buffer = (char *) MemoryAllocator::instance().mem_alloc(sizeof(char) * KB_SIZE);
        itemAvailable= Sem::open(0);
    }

    ~KeyboardBuffer(){
        delete buffer;
        sem_close(itemAvailable);
    }
    char get();
    void put(char c);
    size_t getCount();

    void *operator new(size_t n);
    void *operator new[](size_t n);
    void operator delete(void *p)noexcept;
    void operator delete[](void *p)noexcept ;
    Sem* itemAvailable;

    static const size_t KB_SIZE = 256;

private:

    char* buffer;
    size_t head, tail, count;
};

#endif
