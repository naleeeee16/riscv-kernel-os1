#ifndef _RISCV_HPP
#define _RISCV_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/trap.hpp"
#include "../h/buffer.hpp"
#include "../h/list.hpp"

class RiscV {
public:

    static void init();

    static void endProgram();

    static void dealocateAll();


    static TCB *put;
    static bool end;

    static KeyboardBuffer *inBuff;
    static KeyboardBuffer *outBuff;

    static void packArgumentsAndEcall(int num, uint64 a1, uint64 a2, uint64 a3, uint64 a4);

    static void idle(void *);

    static void putBuff(void *);


    // pop sstatus.spp and sstatus.spie bits (has to be a non inline function)
    static void popSppSpie();

    static uint64 r_scause();

    static void w_scause(uint64 scause);

    static uint64 r_sepc();

    static void w_sepc(uint64 sepc);

    static uint64 r_stvec();

    static void w_stvec(uint64 stvec);

    static uint64 r_stval();

    static void w_stval(uint64 stval);

    enum BitMaskSip {
        SIP_SSIP = (1 << 1),
        SIP_STIP = (1 << 5), // ne treba
        SIP_SEIP = (1 << 9),
    };

    static uint64 r_sie();

    static void ms_sie(uint64 mask);

    static void mc_sie(uint64 mask);

    static uint64 r_sip();

    static void w_sip(uint64 sip);

    static void mc_sip(uint64 mask);

    static void ms_sip(uint64 mask);

    enum BitMaskSstatus {
        SSTATUS_SIE = (1 << 1),
        SSTATUS_SPIE = (1 << 5),
        SSTATUS_SPP = (1 << 8),
    };

    static void ms_sstatus(uint64 mask);

    static void mc_sstatus(uint64 mask);

    static uint64 r_sstatus();

    static void w_sstatus(uint64 sstatus);

    static void w_a0(uint64 x);

    static uint64 r_a0();

    static uint64 r_a1();

    static uint64 r_a2();

    static List<Sem> * semaphores;
    static List<TCB> * threads;

};

inline void RiscV::w_a0(uint64 x) {
    __asm__ volatile ("mv a0, %0"::"r" (x));
}

inline uint64 RiscV::r_a0() {
    volatile uint64 a0; // da li treba volatile??
    __asm__ volatile ("mv %0, a0" : "=r" (a0));
    return a0;
}

inline uint64 RiscV::r_a1() {
    volatile uint64 a0; // da li treba volatile??
    __asm__ volatile ("mv %0, a1" : "=r" (a0));
    return a0;
}

inline uint64 RiscV::r_a2() {
    volatile uint64 a0; // da li treba volatile??
    __asm__ volatile ("mv %0, a2" : "=r" (a0));
    return a0;
}

inline uint64 RiscV::r_scause() {
    uint64 volatile scause;
    __asm__ volatile ("csrr %[scause], scause" : [scause] "=r"(scause));
    return scause;
}

inline void RiscV::w_scause(uint64 scause) {
    __asm__ volatile ("csrw scause, %[scause]" : : [scause] "r"(scause));
}

inline uint64 RiscV::r_sepc() {
    uint64 volatile sepc;
    __asm__ volatile ("csrr %[sepc], sepc" : [sepc] "=r"(sepc));
    return sepc;
}


inline uint64 RiscV::r_sie() {
    uint64 volatile sie;
    __asm__ volatile ("csrr %[sie], sie" : [sie] "=r"(sie));
    return sie;
}

inline void RiscV::w_sepc(uint64 sepc) {
    __asm__ volatile ("csrw sepc, %[sepc]" : : [sepc] "r"(sepc));
}

inline uint64 RiscV::r_stvec() {
    uint64 volatile stvec;
    __asm__ volatile ("csrr %[stvec], stvec" : [stvec] "=r"(stvec));
    return stvec;
}

inline void RiscV::w_stvec(uint64 stvec) {
    __asm__ volatile ("csrw stvec, %[stvec]" : : [stvec] "r"(stvec));
}

inline uint64 RiscV::r_stval() {
    uint64 volatile stval;
    __asm__ volatile ("csrr %[stval], stval" : [stval] "=r"(stval));
    return stval;
}

inline void RiscV::w_stval(uint64 stval) {
    __asm__ volatile ("csrw stval, %[stval]" : : [stval] "r"(stval));
}

inline void RiscV::ms_sip(uint64 mask) {
    __asm__ volatile ("csrs sip, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::ms_sie(uint64 mask) {
    __asm__ volatile ("csrs sie, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sie(uint64 mask) {
    __asm__ volatile ("csrc sie, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sip(uint64 mask) {
    __asm__ volatile ("csrc sip, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sip() {
    uint64 volatile sip;
    __asm__ volatile ("csrr %[sip], sip" : [sip] "=r"(sip));
    return sip;
}

inline void RiscV::w_sip(uint64 sip) {
    __asm__ volatile ("csrw sip, %[sip]" : : [sip] "r"(sip));
}

inline void RiscV::ms_sstatus(uint64 mask) {
    __asm__ volatile ("csrs sstatus, %[mask]" : : [mask] "r"(mask));
}

inline void RiscV::mc_sstatus(uint64 mask) {
    __asm__ volatile ("csrc sstatus, %[mask]" : : [mask] "r"(mask));
}

inline uint64 RiscV::r_sstatus() {
    uint64 volatile sstatus;
    __asm__ volatile ("csrr %[sstatus], sstatus" : [sstatus] "=r"(sstatus));
    return sstatus;
}

inline void RiscV::w_sstatus(uint64 sstatus) {
    __asm__ volatile ("csrw sstatus, %[sstatus]" : : [sstatus] "r"(sstatus));
}

#endif //PROJECT_BASE_V1_1_RISCVREGUTIL_HPP
