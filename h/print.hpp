#ifndef _PRINTING_HPP_
#define _PRINTING_HPP_

#include "syscall_c.hpp"

typedef unsigned long uint64;

extern "C" uint64 cpy_and_swp(uint64 &lock, uint64 expected, uint64 desired);

void printStr(char const *string);

char* getStr(char *buf, int max);

int striToInt(const char *s);

void printInteger(int xx, int base=10, int sgn=0);

#endif // _PRINTING_HPP_

