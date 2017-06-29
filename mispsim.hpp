#ifndef MISPSIM_H
#define MISPSIM_H
#include <bits/stdc++.h>
using namespace std;

long long reg[32];
void (*instp)[64];
byte inst[10];
long long pc, lo, hi;
siminit(int mp)
{
	for (int i = 0; i < 32; ++i)
	{
		reg[i] = 0;
	}
	pc = mp;
}
void IF()
{
	for (int i = 0; i < 8; ++i)
		inst[i] = data[pc++];
}
int pd[3];
int *pa[3];
void ID()
{
	int op = inst[0];
	if (IsRegI(op))
	{
		pd=inst[1];
	}
	else if (IsImmI(op))
	{

	}
	else if (IsJmpI(op))
	{

	}
	else
	{

	}
}

void EX()
{
	instp(
}
void MA()
{

}
void WB()
{

}
void run()
{
	cerr << "simulation start" << endl;
	while (1)
	{
		IF();
	}
}
void halt()
{
	cerr << "simulation halt" << endl;
}

#endif

