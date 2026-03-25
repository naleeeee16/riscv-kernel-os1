#include "../h/syscall_cpp.hpp"
#include "../h/tcb.hpp"

void wrappRun(void *t) {
    ((Thread *) t)->run();
}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle = nullptr;
    this->body = body;
    this->arg = arg;
}

Thread::~Thread() noexcept {
    delete myHandle;
}

int Thread::start() {
    if (myHandle == nullptr) {
        return thread_create(&myHandle, body, arg);
    }
    return -1;
}

int Thread::sleep(time_t t) {
    return time_sleep(t);
}

void Thread::join() {
    thread_join(myHandle);
}

void Thread::dispatch() {
    thread_dispatch();
}

Thread::Thread() {
    myHandle = nullptr;
    body = wrappRun;
    arg = this;
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}
// da li je ovo to
void PeriodicThread::terminate() {
    delete this;
}
PeriodicThread::PeriodicThread(time_t period) :Thread(), period(period){}

void PeriodicThread::run() {
    while(1){
        periodicActivation();
        time_sleep(period);
    }
}

void Console::putc(char c) {
    ::putc(c);
}

char Console::getc() {
    return ::getc();
}
