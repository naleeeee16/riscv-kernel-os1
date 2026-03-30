# RISC-V Kernel Implementation 
## Operating Systems 1 (OS1) | University of Belgrade School of Electrical Engineering | August 2023
This repository contains the source code for a 64-bit RISC-V kernel, developed as a core project for the Operating Systems course.
The kernel runs in RISC-V supervisor mode and provides thread management, synchronisation primitives, memory allocation, and console I/O.

## Key Features

### Memory management
- Custom heap allocator using a **best-fit algorithm** with two doubly-linked lists (free and used blocks)
- Free block **coalescing** (`try_to_join`) — adjacent free blocks are merged automatically to prevent fragmentation
- C++ `operator new` and `operator delete` wired directly to the custom allocator

### Thread management and Scheduling
- **Context switching:** Low-level register save/restore logic implemented in RISC-V assembly (contextSwitch.S).
- `thread_create`, `thread_exit`, `thread_dispatch`, `thread_join` system calls
- **Round-robin preemptive scheduler** with configurable time slices
- **Sleeping scheduler** — separate linked list for time-blocked threads, woken by timer interrupt

### Synchronisation and Interupts
- **Semaphores:** Classic wait/signal implementation with a FIFO BlockedList.
- **Spinlock** implemented in RISC-V assembly using `lr.w`/`sc.w` (load-reserved / store-conditional) for atomic compare-and-swap (`lck.S`)
- Vectored trap table (`table.S`) dispatching to three handlers:
  - **Syscall trap** — environment calls from U-mode (8) and S-mode (9)
  - **Timer trap** — preemption and sleeping thread wake-up
  - **Console trap** — UART receive interrupt via PLIC

### Console I/O
- Interrupt-driven UART communication with ring buffers.
- Input buffer uses semaphores to block reader threads until data is available.


---

## Project files

```
├── h/                  Header files
│   ├── riscv.hpp       RISC-V CSR access, hardware constants
│   ├── tcb.hpp         Thread Control Block
│   ├── scheduler.hpp   Round-robin scheduler
│   ├── sleepingscheduler.hpp
│   ├── sem.hpp         Semaphore
│   ├── memoryallocator.hpp
│   ├── blockedlist.hpp
│   ├── buffer.hpp      Keyboard ring buffer
│   ├── trap.hpp        Trap handlers
│   ├── syscall_c.hpp   C syscall wrappers
│   └── syscall_cpp.hpp C++ syscall wrappers (Thread, Semaphore classes)
├── src/                Implementation files
│   ├── contextSwitch.S RISC-V context switch (ra, sp save/restore)
│   ├── lck.S           Atomic compare-and-swap spinlock
│   ├── table.S         Vectored trap dispatch table
│   ├── main.cpp        Kernel entry point
│   ├── tcb.cpp
│   ├── scheduler.cpp
│   ├── sleepingscheduler.cpp
│   ├── sem.cpp
│   ├── memoryallocator.cpp
│   ├── blockedlist.cpp
│   ├── buffer.cpp
│   ├── riscv.cpp
│   ├── trap.cpp
│   ├── print.cpp
│   ├── syscall_c.cpp
│   ├── syscall_cpp.cpp
│   └── _new.cpp        Global operator new/delete

```
## Technologies

C++, RISC-V Assembly (RV64G), QEMU virt machine
