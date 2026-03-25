#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"

void* mem_alloc(size_t size) {
    size_t sizeBlocks= (size+ MEM_BLOCK_SIZE-1)/MEM_BLOCK_SIZE;
    RiscV::packArgumentsAndEcall(0x01,sizeBlocks,2,3,4);
    return (void*)RiscV::r_a0();
}

int mem_free(void* x) {
    RiscV::packArgumentsAndEcall(0x02,(uint64)x,0,0,0);
    return RiscV::r_a0();
}
int thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    void* stack= nullptr;
    if(start_routine!= nullptr){
        stack= mem_alloc(sizeof(uint64)*DEFAULT_STACK_SIZE);
        if(stack== nullptr)return -1; // greska
   }
    RiscV::packArgumentsAndEcall(0x11, (uint64)handle, (uint64)start_routine, (uint64 )arg,(uint64 )stack);
    return RiscV::r_a0();
}


int thread_exit (){
    RiscV::packArgumentsAndEcall(0x12,0,0,0,0);
    return RiscV::r_a0();
};

void thread_dispatch(){
    RiscV::packArgumentsAndEcall(0x13,0,0,0,0);
    return;
}

void thread_join(thread_t handle){
    RiscV::packArgumentsAndEcall(0x14,(uint64)handle,0,0,0);
}


int sem_open(sem_t* handle , unsigned init){
    RiscV::packArgumentsAndEcall(0x21,(uint64)handle,(uint64)init,0,0);
    return RiscV::r_a0();
}

int sem_close(sem_t handle){
    RiscV::packArgumentsAndEcall(0x22,(uint64)handle,0,0,0);
    return RiscV::r_a0();
}

int sem_wait(sem_t id){ //0x23
    RiscV::packArgumentsAndEcall(0x23,(uint64)id,0,0,0);
    return RiscV::r_a0();
}

int sem_signal(sem_t id){
    RiscV::packArgumentsAndEcall(0x24,(uint64)id,0,0,0);
    return RiscV::r_a0();
}


int time_sleep(time_t time){
    RiscV::packArgumentsAndEcall(0x31,(uint64)time,0,0,0);
    return RiscV::r_a0();
}

char getc (){
    RiscV::packArgumentsAndEcall(0x41,0,0,0,0);
    return RiscV::r_a0();
}

void putc( char c){
    RiscV::packArgumentsAndEcall(0x42,c,0,0,0);
}