#ifndef GLOBAL_H_
#define GLOBAL_H_
typedef char byte;
typedef short half;
typedef int word;

const int M = 1 << 22;
byte data[M];
const int TEXT = 0x500;
const int DATA = 0x150000;
int text_p = TEXT;
int data_p = DATA;
#endif
