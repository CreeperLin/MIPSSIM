#ifndef GLOBAL_H_
#define GLOBAL_H_
#include <bits/stdc++.h>
using namespace std;
const int M = 1 << 22;
typedef char byte;
typedef short half;
typedef int word;
void (*insp[64])(int *ret, long long *pd);
byte data[M];

#define DEBUG 0
#define PIPE 1
#define MANUAL 0
const int TEXT = 0x500;
const int DATA = 0x150000;
int text_p = TEXT;
int data_p = DATA;

ofstream fstat;
ifstream fas;
#if MANUAL
ofstream fout;
ifstream fin;
#else
ostream &fout=cout;
istream &fin=cin;
#endif
char numstr[][5] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
char regstr[][10] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "k0", "k1", "gp", "sp", "s8", "ra", "lo", "hi", "pc"};
char assstr[][10] = {"align", "ascii", "asciiz", "byte", "half", "word", "space", "data", "text"};
char insstr[][10] = {"add", "addu", "addiu", "sub", "subu", "mul", "mulu", "div", "divu", "xor", "xoru", "neg", "negu", "rem", "remu", "li", "seq", "sge", "sgt", "sle", "slt", "sne", "move", "mfhi", "mflo", "la", "lb", "lh", "lw", "sb", "sh", "sw", "b", "beq", "bne", "bge", "ble", "bgt", "blt", "beqz", "bnez", "blez", "bgez", "bgtz", "bltz", "j", "jr", "jal", "jalr", "nop", "syscall"};
#endif
