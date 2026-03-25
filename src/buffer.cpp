#include "../h/buffer.hpp"

size_t KeyboardBuffer::getCount() { return count; }

char KeyboardBuffer::get() {

    sem_wait(itemAvailable);
    char c = buffer[head];
    head = (head + 1) % KB_SIZE;
    count--;
    return c;
}

void KeyboardBuffer::put(char c) {
    buffer[tail] = c;
    tail = (tail + 1) % KB_SIZE;
    count++;
    itemAvailable->signal();

}

void *KeyboardBuffer::operator new(size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void *KeyboardBuffer::operator new[](size_t n) {
    return MemoryAllocator::instance().mem_alloc(n);
}

void KeyboardBuffer::operator delete(void *p) noexcept {
    MemoryAllocator::instance().mem_free(p);
}

void KeyboardBuffer::operator delete[](void *p) noexcept {
    MemoryAllocator::instance().mem_free(p);
}
