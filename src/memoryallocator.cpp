#include "../h/memoryallocator.hpp"
#include "../h/print.hpp"

BlockMem *MemoryAllocator::freeMemHead = nullptr;
BlockMem *MemoryAllocator::usedMemHead = nullptr;

int MemoryAllocator::mem_free(void *ptr) {
    BlockMem *toFree = (BlockMem *) ((uint64) ptr - sizeof(BlockMem));
    bool found = false;
    BlockMem *curr = usedMemHead;
    while (curr != nullptr) {
        if ((uint64) toFree == (uint64) curr) {
            found = true;
            //printString("found ->delete ");
        }
        if (found)break;
        curr = curr->next;
    }

    if (!found)return -1;

    BlockMem *newFree = curr;
    BlockMem *prevUsed = curr->prev;
    BlockMem *nextUsed = curr->next;

    if (prevUsed == nullptr) {
        if (nextUsed != nullptr) {
            usedMemHead = nextUsed;
        } else {
            usedMemHead = nullptr;
        }
    } else {
        prevUsed->next = nextUsed;
        if (nextUsed != nullptr) {
            nextUsed->prev = prevUsed;
        }
    }
    newFree->next = nullptr;
    newFree->prev = nullptr;
    BlockMem *currFree = freeMemHead;
    BlockMem *prev = nullptr;
    while (currFree != nullptr) {
        if ((uint64) newFree < (uint64) currFree) break;
        prev = currFree;
        currFree = currFree->next;
    }
    if (currFree != nullptr) {
        if (currFree->prev == nullptr) {
            freeMemHead = newFree;
        } else {
            currFree->prev->next = newFree;
            newFree->prev = currFree->prev;
        }
        currFree->prev = newFree;
        newFree->next = currFree;
    } else {
        if (prev != nullptr) {
            prev->next = newFree;
            newFree->prev = prev;
        } else {
            freeMemHead = newFree;
        }
    }
    try_to_join(newFree);
    return 0;
}

void *MemoryAllocator::mem_alloc(size_t size) {

    size_t MAX_SIZE = (size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR - sizeof(BlockMem) +1;

    if (size == 0 || size > MAX_SIZE)return nullptr;
    size += sizeof(BlockMem);
    size = sizeMemBlock(size) - sizeof(BlockMem);

    BlockMem *curr = freeMemHead;
    BlockMem *best = nullptr;
    size_t best_size = MAX_SIZE;
//BEST FIT ALGORITAM
    while (curr != nullptr) {
        if (curr->size - size >= 0) {
            size_t size1 = curr->size - size;
            if (size1 < best_size) {
                best_size = size1;
                best = curr;
            }
        }
        curr = curr->next;
    }
    if (best == nullptr) {
        //printString("Nema memorije:(");
        return nullptr;
    }

    BlockMem *newUsed = best;
    newUsed->size = size;
    BlockMem *freePrev = best->prev;
    BlockMem *freeNext = best->next;

    newUsed->next = nullptr;
    newUsed->prev = nullptr;
    if (usedMemHead == nullptr) {
        usedMemHead = (BlockMem *) newUsed;
        usedMemHead->next = nullptr;
        usedMemHead->prev = nullptr;
    } else {
        BlockMem *curr = usedMemHead;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = newUsed;
        newUsed->prev = curr;
        newUsed->next = nullptr;
    }
    if (best_size == 0 || best_size < sizeof(BlockMem) + MEM_BLOCK_SIZE) {
        if (freePrev == nullptr) {
            if (freeNext == nullptr) {
                freeMemHead = nullptr;
                //printString("Puno je ");
            } else {
                freeNext->prev = nullptr;
                freeMemHead = freeNext;
            }
        } else {
            freePrev->next = freeNext;
            if (freeNext != nullptr) {
                freeNext->prev = freePrev;
            }
        }
        newUsed->size += best_size;
    } else {
        BlockMem *newFree = (BlockMem *) ((uint64) newUsed + sizeof(BlockMem) + newUsed->size);
        newFree->size = best_size - sizeof(BlockMem);


        if (freePrev == nullptr) {
            freeMemHead = newFree;

        } else {
            freePrev->next = newFree;
        }
        newFree->prev = freePrev;
        newFree->next = freeNext;

        if (freeNext != nullptr) {
            freeNext->prev = newFree;
        }
    }
    return (uint64 *) ((uint64) newUsed + sizeof(BlockMem));
}

size_t MemoryAllocator::sizeMemBlock(size_t size) {
    if (size % MEM_BLOCK_SIZE == 0) {
        return size;
    }
    return (size / MEM_BLOCK_SIZE + 1) * MEM_BLOCK_SIZE;
}

void MemoryAllocator::try_to_join(BlockMem *freeNode) {
    if (freeNode == nullptr)return;
    BlockMem *prev = freeNode->prev;
    BlockMem *next = freeNode->next;

    if (next != nullptr) {// levi
        if ((uint64) freeNode + freeNode->size + sizeof(BlockMem) == (uint64) next) {
            freeNode->size += next->size + sizeof(BlockMem);
            freeNode->next = next->next;
            if (next->next != nullptr) {
                next->prev = freeNode;
            }
        }
    }
    if (prev != nullptr) {//desni
        if ((uint64) prev + prev->size + sizeof(BlockMem) == (uint64) freeNode) {
            prev->size += freeNode->size + sizeof(BlockMem);
            prev->next = freeNode->next;
            if (freeNode->next != nullptr) {
                freeNode->prev = prev;
            }
        }
    }
}

MemoryAllocator &MemoryAllocator::instance() {
    static MemoryAllocator *myAllocator;
    static bool initialized = false;
    if (!initialized) {
        freeMemHead = (BlockMem *) ((size_t) HEAP_START_ADDR);//+ )sizeof(myAllocator);
        size_t MAX_SIZE = (size_t) HEAP_END_ADDR - (size_t) HEAP_START_ADDR+1- sizeof(BlockMem);
        freeMemHead->size = MAX_SIZE;
        freeMemHead->next = nullptr;
        freeMemHead->prev = nullptr;
        usedMemHead = nullptr;
        initialized = true;
    }
    return *myAllocator;
}
