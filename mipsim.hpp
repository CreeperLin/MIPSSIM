#ifndef MIPSSIM_H
#define MIPSSIM_H
#include <bits/stdc++.h>
#include "main.h"
using namespace std;
#define cerra if(DEBUG) cerr

//class mipsim
//{

long long cpuclk;

//public:

void IF(unsigned char *ins, long long &pc)
{
	cerra << "IF: ";
	memcpy(ins, data + pc, 12);
	for (int i = 0; i < 12; ++i) cerra << (int)ins[i] << ' ';
	cerra << endl;
	pc += 12;
}



void ID(long long *pd, unsigned char* ins, long long *reg)
{
	cerra << "ID: ";
	int op = ins[0];
	if (op < 24)
	{
		cerra << "R:";
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : 0);
		pd[2] = (ins[7] == 1) ? (*reinterpret_cast<word*>(ins + 8)) : ((ins[7] == 2) ? reg[ins[8]] : 0);
		pd[3] = ins[1];
		pd[4] = ins[7];
	}
	else if (op == 24) pd[3] = ins[1], pd[1] = reg[33];
	else if (op == 25) pd[3] = ins[1], pd[1] = reg[32];
	else if (op < 33)
	{
		cerra << "M:";
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 2) ? reg[ins[3]] : 0;
		pd[2] = *reinterpret_cast<word*>(ins + 8);
		pd[3] = ins[1];
	}
	else if (op < 50)
	{
		cerra << "J:";
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : reg[34]);
		pd[2] = *reinterpret_cast<word*>(ins + 8);
	}
	else if (op < 52)
	{
		cerra << "O:";
		if (ins[0] == 51)
		{
			pd[0] = reg[2];
			pd[1] = reg[4];
			pd[2] = reg[5];
		}
	}
	else
	{
		cerra << '!' << endl;
		exit(1);
	}
	cerra << '\t' << pd[0] << ' ' << pd[1] << ' ' << pd[2] << endl;
//	return op;
}

void EX(int *ret, int op, long long *pa)
{
	cerra << "EX: " << op << endl;
	ret[0] = ret[1] = 0;
	if (op)
	{
		insp[op](ret, pa);
	}
}

void MA(int *ret, int *pa)
{
	cerra << "MA";
	ret[0] = 0;
	if (pa[0] == 2)
	{
		cerra << ": R:" << pa[4] << " reg:" << pa[3] << " maddr:" << pa[2];
		ret[0] = 1;
		ret[1] = pa[3];
		switch (pa[4])
		{
			case 1:
				ret[2] = *reinterpret_cast<byte*>(data + pa[2]);
				break;
			case 2:
				ret[2] = *reinterpret_cast<half*>(data + pa[2]);
				break;
			case 4:
				ret[2] = *reinterpret_cast<word*>(data + pa[2]);
				break;
			default:
				exit(2);
		}
		cerra << " val:" << ret[2];
//		wbval[wbcnt] = 0;
//		memcpy(wbval + wbcnt, data + maddr, masiz);
	}
	else if (pa[0] == 3)
	{
		cerra << ": W:" << pa[4] << " val:" << pa[3] << " maddr:" << pa[2];
		memcpy(data + pa[2], pa + 3, pa[4]);
	}
	cerra << endl;
}
void WB(int &wbcnt, int *wbreg, int *wbval, long long *reg)
{
	cerra << "WB";
	for (int i = 0; i < wbcnt; ++i)
	{
		cerra << ": reg:" << wbreg[i] << " val:" << wbval[i];
		reg[wbreg[i]] = wbval[i];
	}
	wbcnt = 0;
	cerra << endl;
}
void halt()
{
	cerr << "simulation halt: cpuclk:" << cpuclk << endl;
}

void run(int mp)
{
	cpuclk = 0;
	long long reg[35] = {0};
	long long &pc = reg[34], &sp = reg[29];
	pc = mp;
	sp = M - (1 << 8);
	cerr << "simulation start" << endl;

	unsigned char ins[15];
	long long pd[6];
	int ret[10];
	int mret[5];

	int wbcnt = 0, wbreg[5], wbval[5];
	while (pc)
	{
		cerra << '>' << cpuclk << " pc:" << pc << ' ' << insstr[data[pc] - 1] << '{' << endl;
		IF(ins, pc);

		ID(pd, ins, reg);

		int op = ins[0];
		EX(ret, op, pd);
		if (ret[0] == 1)
		{
			assert(wbcnt == 0);
			wbreg[0] = ret[2];
			wbval[0] = ret[3];
			wbcnt = 1;
			if (ret[1] == 2)
			{
				wbreg[1] = ret[4];
				wbval[1] = ret[5];
				wbcnt = 2;
			}
		}
		MA(mret, ret);
		if (mret[0])
		{
			wbreg[0] = mret[1];
			wbval[0] = mret[2];
			wbcnt = 1;
		}
		WB(wbcnt, wbreg, wbval, reg);
		cerra << '}' << endl;
		++cpuclk;
	}
	halt();
}

//};
#endif

