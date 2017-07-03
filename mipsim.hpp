#ifndef MIPSSIM_H
#define MIPSSIM_H
#include <bits/stdc++.h>
#include "main.h"
using namespace std;

//class mipsim
//{

long long cpuclk;

//public:

void IF(unsigned char *ins, long long &pc)
{
	cerr << "IF: ";
	memcpy(ins, data + pc, 12);
	for (int i = 0; i < 12; ++i) cerr << (int)ins[i] << ' ';
	cerr << endl;
	pc += 12;
}



void ID(long long *pd, unsigned char* ins, long long *reg)
{
	cerr << "ID: ";
	int op = ins[0];
	if (op < 24)
	{
		cerr << "R:";
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
		cerr << "M:";
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 2) ? reg[ins[3]] : 0;
		pd[2] = *reinterpret_cast<word*>(ins + 8);
		pd[3] = ins[1];
	}
	else if (op < 50)
	{
		cerr << "J:";
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : reg[34]);
		pd[2] = *reinterpret_cast<word*>(ins + 8);
	}
	else if (op < 52)
	{
		cerr << "O:";
		if (ins[0] == 51)
		{
			pd[0] = reg[2];
			pd[1] = reg[4];
			pd[2] = reg[5];
		}
	}
	else
	{
		cerr << '!' << endl;
		exit(1);
	}
	cerr << '\t' << pd[0] << ' ' << pd[1] << ' ' << pd[2] << endl;
//	return op;
}

void EX(int *ret, int op, long long *pa)
{
	cerr << "EX: " << op << endl;
	ret[0] = ret[1] = 0;
	if (op)
	{
		insp[op](ret, pa);
	}
}

void MA(int *ret, int *pa)
{
	cerr << "MA";
	ret[0] = 0;
	if (pa[0] == 2)
	{
		cerr << ": R:" << pa[4] << " reg:" << pa[3] << " maddr:" << pa[2];
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
		cerr << " val:" << ret[2];
//		wbval[wbcnt] = 0;
//		memcpy(wbval + wbcnt, data + maddr, masiz);
	}
	else if (pa[0] == 3)
	{
		cerr << ": W:" << pa[4] << " val:" << pa[3] << " maddr:" << pa[2];
		memcpy(data + pa[2], pa + 3, pa[4]);
	}
	cerr << endl;
}
void WB(int &wbcnt, int *wbreg, int *wbval, long long *reg)
{
	cerr << "WB";
	for (int i = 0; i < wbcnt; ++i)
	{
		cerr << ": reg:" << wbreg[i] << " val:" << wbval[i];
		reg[wbreg[i]] = wbval[i];
	}
	wbcnt = 0;
	cerr << endl;
}
void halt()
{
	cerr << "simulation halt: cpuclk:" << cpuclk << endl;
}

void run(int mp)
{
	cpuclk = 0;
	long long reg[35];
	long long &pc = reg[34], &sp = reg[29];
	for (int i = 0; i < 35; ++i)
	{
		reg[i] = 0;
	}
	pc = mp;
	sp = M - (1 << 8);
	cerr << "simulation start" << endl;

	unsigned char ins[15];
	long long pd[6];
	int ret[10];
	int mret[5];

	int wbcnt, wbreg[5], wbval[5];
	while (pc)
	{
		cerr << '>' << cpuclk << " pc:" << pc << ' ' << insstr[data[pc] - 1] << '{' << endl;
		IF(ins, pc);

		ID(pd, ins, reg);

		int op = ins[0];
		EX(ret, op, pd);
		if (ret[0]==1)
		{
			assert(wbcnt==0);
			wbreg[wbcnt] = ret[2];
			wbval[wbcnt++] = ret[3];
			if (ret[1]==2)
			{
				wbreg[wbcnt] = ret[4];
				wbval[wbcnt++] = ret[5];
			}
		}
		MA(mret, ret);
		if (mret[0])
		{
			wbreg[wbcnt] = mret[1];
			wbval[wbcnt++] = mret[2];
		}
		WB(wbcnt, wbreg, wbval, reg);
		cerr << '}' << endl;
		++cpuclk;
	}
	halt();
}

//};
#endif

