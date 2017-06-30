#ifndef MIPSSIM_H
#define MIPSSIM_H
#include <bits/stdc++.h>
using namespace std;
int pd[3], pr[3];
long long int *pa;
//class mipsim
//{
long long reg[35];
long long &pc=reg[34];

byte ins[10];
//public:
mipsim(int mp)
{
	for (int i = 0; i < 32; ++i)
	{
		reg[i] = 0;
	}
	pc = mp;
}
void IF()
{
	memcpy(ins, data + pc, 8);
	pc += 8;
}
void ID()
{
	int op = ins[0];
	if (op & (1<<7))
	{
		cout << 'R' << endl;
		pa = reg + ins[1];
		pr[0] = ins[1];
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<half*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : 0);
		pd[2] = (ins[5] == 1) ? (*reinterpret_cast<half*>(ins + 6)) : ((ins[5] == 2) ? reg[ins[6]] : 0);
	}
	else if (op & (1<<6))
	{
		cout << 'J' << endl;
		pd[0] = reg[ins[1]];
		if (ins[2] == 2)
		{
			pd[1] = reg[ins[3]];
			pd[2] = *reinterpret_cast<half*>(ins + 6);
		}
		else
		{
			pd[1] = -1;
			pd[2] = *reinterpret_cast<word*>(ins + 4);
		}
	}
	else
	{
		cout << 'O' << endl;
	}
}

void EX()
{
	wbcnt=0;
	insp[ins[0]]();
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

//};
#endif

