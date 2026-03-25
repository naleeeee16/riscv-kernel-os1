#include "../lib/hw.h"
#include "../h/syscall_cpp.hpp"
#include "../h/syscall_c.hpp"
#include "../h/print.hpp"
#include "../h/riscv.hpp"

static thread_t t1;

void userMain();

void user(void *arg) {

    printStr("UserMain start\n");
    char c='l';
     while(c!='a'){
       printStr("\nUnesi znak: ");
         c= Console::getc();
         Console::putc(c);
     }
    userMain();
    printStr("UserMain done\n");
}

void main() {
    RiscV::init();

    thread_create(&t1, user, (void *) 1);
    thread_join(t1);

    RiscV::dealocateAll();
    //da li treba da pise na kraju proccess finished...

}

