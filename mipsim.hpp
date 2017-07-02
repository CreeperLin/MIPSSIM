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
mipsim(int mp)
{
	cpuclk = 0;
	for (int i = 0; i < 32; ++i)
	{
		reg[i] = 0;
	}
	pc = mp;
	sp = 1 << 21;
}


unsigned char ins[15];
void IF()
{
	//cout << "IF: ";
	memcpy(ins, data + pc, 12);
//	for (int i = 0; i < 12; ++i) //cout << (int)ins[i] << ' ';
	//cout << endl;
	pc += 12;
}

long long pd[3];
int pr[3];

void ID()
{
	//cout << "ID: ";
	int op = ins[0];
	if (op < 24)
	{
		//cout << "R:";
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
		//cout << "M:";
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
		//cout << "J:";
		pd[0] = reg[ins[1]];
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : 0);
		pd[2] = *reinterpret_cast<word*>(ins + 8);
	}
	else if (op < 52)
	{
		//cout << "O:";
		if (ins[0] == 51)
		{
			pd[0] = reg[2];
			pd[1] = reg[4];
			pd[2] = reg[5];
		}
	}
	else
	{
		//cout << '!' << endl;
		exit(1);
	}

	//cout << '\t' << pd[0] << ' ' << pd[1] << ' ' << pd[2] << endl;
}

void EX()
{
	//cout << "EX: " << (int)ins[0] << endl;
	wbcnt = 0;
	maflg = 0;
	insp[ins[0]]();
}

void MA()
{
	//cout << "MA";
	if (maflg == 1)
	{
		//cout << ": R:" << masiz << " reg:" << maval << " val:" << *reinterpret_cast<int*>(data + maddr) << " maddr:" << maddr;
		wbreg[wbcnt] = maval;
		wbval[wbcnt] = 0;
		memcpy(wbval + wbcnt, data + maddr, masiz);
		++wbcnt;
	}
	else if (maflg == 2)
	{
		//cout << ": W:" << masiz << " val:" << maval << " maddr:" << maddr;
		memcpy(data + maddr, &maval, masiz);
	}
	//cout << endl;
}
void WB()
{
	//cout << "WB";
	for (int i = 0; i < wbcnt; ++i)
	{
		//cout << ": reg:" << wbreg[i] << " val:" << wbval[i];
		reg[wbreg[i]] = wbval[i];
	}
	//cout << endl;
}

void halt()
{
	//cout << "simulation halt" << endl;
}

int run()
{
	//cout << "simulation start" << endl;
	while (pc)
	{
		//cout << '>' << cpuclk << " pc:" << reg[34] << ' ' << insstr[data[pc] - 1] << '{' << endl;
		IF();
		ID();
		EX();
		MA();
		WB();
		//cout << '}' << endl;
		++cpuclk;
	}
	halt();
	return retval;
}

//};
#endif

