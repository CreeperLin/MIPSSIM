#ifndef MIPSSIM_H
#define MIPSSIM_H
#include <bits/stdc++.h>
#include "main.h"
using namespace std;

//class mipsim
//{
long long reg[35];
long long &pc = reg[34], &sp = reg[29];
long long cpuclk;

long long wbval[5];
int wbreg[5], wbcnt;
int maflg, masiz, maval, maddr;

int retval;

//public:
void mipsim(int mp)
{
	cpuclk = 0;
	for (int i = 0; i < 32; ++i)
	{
		reg[i] = 0;
	}
	pc = mp;
	sp = M - (1 << 8);
}


unsigned char ins[15];
void IF()
{
	//cerr << "IF: ";
	memcpy(ins, data + pc, 12);
//	for (int i = 0; i < 12; ++i) //cerr << (int)ins[i] << ' ';
	//cerr << endl;
	pc += 12;
}

long long pd[3];
int pr[3];

void ID()
{
	//cerr << "ID: ";
	int op = ins[0];
	if (op < 24)
	{
		//cerr << "R:";
		pr[0] = ins[1];
		pr[1] = ins[7];
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : 0);
		pd[2] = (ins[7] == 1) ? (*reinterpret_cast<word*>(ins + 8)) : ((ins[7] == 2) ? reg[ins[8]] : 0);
	}
	else if (op == 24) pr[0] = ins[1], pd[1] = reg[33];
	else if (op == 25) pr[0] = ins[1], pd[1] = reg[32];
	else if (op < 33)
	{
		//cerr << "M:";
		pr[0] = ins[1];
		pd[0] = reg[ins[1]];
		if (ins[2] == 2)
		{
			pd[1] = reg[ins[3]];
			pd[2] = *reinterpret_cast<word*>(ins + 8);
		}
		else
		{
			pd[1] = 0;
			pd[2] = *reinterpret_cast<word*>(ins + 8);
		}
	}
	else if (op < 50)
	{
		//cerr << "J:";
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : 0);
		pd[2] = *reinterpret_cast<word*>(ins + 8);
	}
	else if (op < 52)
	{
		//cerr << "O:";
		if (ins[0] == 51)
		{
			pd[0] = reg[2];
			pd[1] = reg[4];
			pd[2] = reg[5];
		}
	}
	else
	{
		//cerr << '!' << endl;
		exit(1);
	}

	//cerr << '\t' << pd[0] << ' ' << pd[1] << ' ' << pd[2] << endl;
}

void EX()
{
	//cerr << "EX: " << (int)ins[0] << endl;
	wbcnt = 0;
	maflg = 0;
	insp[ins[0]]();
}

void MA()
{
	//cerr << "MA";
	if (maflg == 1)
	{
		//cerr << ": R:" << masiz << " reg:" << maval << " maddr:" << maddr;
		wbreg[wbcnt] = maval;
		switch (masiz)
		{
			case 1:
				wbval[wbcnt] = *reinterpret_cast<byte*>(data + maddr);
				break;
			case 2:
				wbval[wbcnt] = *reinterpret_cast<half*>(data + maddr);
				break;
			case 4:
				wbval[wbcnt] = *reinterpret_cast<word*>(data + maddr);
				break;
			default:
				exit(2);
		}
		//cerr << " val:" << wbval[wbcnt];
		++wbcnt;
//		wbval[wbcnt] = 0;
//		memcpy(wbval + wbcnt, data + maddr, masiz);
	}
	else if (maflg == 2)
	{
		//cerr << ": W:" << masiz << " val:" << maval << " maddr:" << maddr;
		memcpy(data + maddr, &maval, masiz);
	}
	//cerr << endl;
}
void WB()
{
	//cerr << "WB";
	for (int i = 0; i < wbcnt; ++i)
	{
		//cerr << ": reg:" << wbreg[i] << " val:" << wbval[i];
		reg[wbreg[i]] = wbval[i];
	}
	//cerr << endl;
}

void halt()
{
	cerr << "simulation halt: cpuclk:" << cpuclk << endl;
}

int run()
{
	cerr << "simulation start" << endl;
	while (pc)
	{
		//cerr << '>' << cpuclk << " pc:" << reg[34] << ' ' << insstr[data[pc] - 1] << '{' << endl;
		IF();
		ID();
		EX();
		MA();
		WB();
		//cerr << '}' << endl;
		++cpuclk;
	}
	halt();
	return retval;
}

//};
#endif

