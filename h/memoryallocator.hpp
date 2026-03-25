#ifndef _MEMORYALLOCATOR_HPP
#define _MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

struct BlockMem {
    BlockMem *next = nullptr;
    BlockMem *prev = nullptr;
    size_t size = 0; // velicina bez zaglavlja
};

class MemoryAllocator {

public:

    static MemoryAllocator &instance();

    MemoryAllocator &operator=(const MemoryAllocator &m) = delete;

    MemoryAllocator &operator=(MemoryAllocator &&m) = delete;

    MemoryAllocator(MemoryAllocator &&) = delete;

    MemoryAllocator(const MemoryAllocator &) = delete;

    ~MemoryAllocator() {};

    friend class Trap;

    friend class TCB;

    friend class TestMemory;

    friend class Scheduler;

    friend class Sem;

    friend class BlockedList;

    friend class SleepingScheduler;

    template<typename > friend class List;

    friend class KeyboardBuffer;
    // sve klase koje preklapaju opeartore new i delete

private :
    MemoryAllocator() {};

    void *mem_alloc(size_t size);

    int mem_free(void *ptr);

    static BlockMem *freeMemHead;
    static BlockMem *usedMemHead;

    size_t sizeMemBlock(size_t size);

    void try_to_join(BlockMem *freeNode);
};

#endif
