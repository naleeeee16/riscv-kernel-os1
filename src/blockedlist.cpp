#include "../h/blockedlist.hpp"
#include "../h/memoryallocator.hpp"

void *BlockedList::operator new(size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void *BlockedList::operator new[](size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void BlockedList::operator delete(void *p) noexcept {
    ((BlockedList *) p)->freeList();
    MemoryAllocator::instance().mem_free(p);
}

void BlockedList::operator delete[](void *p) noexcept {
    //DA LI JE OCO SODBRO
    ((BlockedList *) p)->freeList();
    MemoryAllocator::instance().mem_free(p);
}
// stavljanje na pocetak
void BlockedList::put(TCB *t) {
    Blocked *newBlocked = (Blocked *) MemoryAllocator::instance().mem_alloc(sizeof(Blocked));
    if (newBlocked == nullptr)return;
    newBlocked->thread = t;
    if (head == nullptr) {
        head = newBlocked;
        head->next = nullptr;
    } else {
        Blocked *next = head;
        newBlocked->next = next;
        head = newBlocked;
    }
}

// uzimanje sa kraja
TCB *BlockedList::get() {
    Blocked *curr = head;
    Blocked *prev = nullptr;
    if (head == nullptr)return nullptr;
    while (curr->next) {
        prev = curr;
        curr = curr->next;
    }
    if (curr) {
        if (prev == nullptr) {
            head = nullptr;
        } else {
            prev->next = nullptr;
        }
        return curr->thread;
    } else return nullptr;
}

bool BlockedList::isEmpty() {
    return head == nullptr;
}

void BlockedList::freeList() {
    Blocked *curr = head;
    while (curr) {
        Blocked *old = curr;
        curr = curr->next;
        MemoryAllocator::instance().mem_free(old);
    }
    head = nullptr;
}