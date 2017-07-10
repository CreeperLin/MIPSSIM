#ifndef MIPSSIM_H
#define MIPSSIM_H
#include <bits/stdc++.h>
#include <thread>
#include "main.h"
#include "branch_predict.hpp"
using namespace std;
#define cerra if(DEBUG) cerr

long long cpuclk;
int hz_ctl, hz_dat, purge;

//stats
int ctrl_stall_cnt = 0;
int data_stall_cnt = 0;
int bp_ptrue = 0, bp_pfalse = 0, bp_total = 0, bp_twrong = 0, bp_fwrong = 0;
int inscnt;

void ISSUE()
{
	
}

void EXEC()
{
	
}

void WRITE()
{
	
}

void IF(unsigned char *ins, long long &pc)
{
	cerra << "IF: ";
	if (hz_ctl || !pc)
	{
		cerra << endl;
		return;
	}
	memcpy(ins, data + pc, 12);
	for (int i = 0; i < 12; ++i) cerra << (int)ins[i] << ' ';
	cerra << endl;
	pc += 12;
	++inscnt;
}

void ID(int &op, long long *pd, unsigned char* ins, long long *reg)
{
	cerra << "ID: ";
	op = ins[0];
	ins[0] = 0;
	if (!op)
	{
		cerra << endl;
		return;
	}
	cerra << insstr[op - 1] << ':';
	if (op < 24)
	{
		cerra << "R:";
		pd[0] = reg[ins[1]];
//		pd[1] = (!ins[2]) ? 0 : ((ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : reg[ins[3]]);
//		pd[2] = (!ins[7]) ? 0 : ((ins[7] == 1) ? (*reinterpret_cast<word*>(ins + 8)) : reg[ins[8]]);
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : 0);
		pd[2] = (ins[7] == 1) ? (*reinterpret_cast<word*>(ins + 8)) : ((ins[7] == 2) ? reg[ins[8]] : 0);
		pd[3] = ins[1];
		pd[4] = ins[7];
	}
	else if (op == 24) pd[3] = ins[1], pd[1] = reg[33];
	else if (op == 25) pd[3] = ins[1], pd[1] = reg[32];
//	else if (op == 24) pd[3] = ins[1], pd[1] = reg[33];
//	else if (op == 25) pd[3] = ins[1], pd[1] = reg[32];
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
//		pd[1] = (!ins[2]) ? reg[34] : ((ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : reg[ins[3]]);
		pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : reg[34]);
		pd[2] = *reinterpret_cast<word*>(ins + 8);
	}
	else if (op < 52)
	{
		cerra << "O:";
		if (op == 51)
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
}

void EX(int *ret, int &op, long long *pa)
{
	cerra << "EX: " << insstr[op - 1] << endl;
	ret[0] = ret[1] = 0;
	if (op)
	{
		insp[op](ret, pa);
		op = 0;
		if (hz_ctl && !ret[1])
		{
			++bp_twrong;
			hz_ctl = 0;
		}
		else if(!hz_ctl && ret[0] == 1 && ret[2] == 34)
		{
			++bp_fwrong;
			purge = 1;
		}
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
	}
	else if (pa[0] == 3)
	{
		cerra << ": W:" << pa[4] << " val:" << pa[3] << " maddr:" << pa[2];
		memcpy(data + pa[2], pa + 3, pa[4]);
	}
	pa[0] = 0;
	cerra << endl;
}
void WB(int &wbcnt, int *wbreg, int *wbval, long long *reg)
{
	cerra << "WB";
	for (int i = 0; i < wbcnt; ++i)
	{
		cerra << ": reg:" << wbreg[i] << " val:" << wbval[i];
		reg[wbreg[i]] = wbval[i];
		if (wbreg[i] == 34) hz_ctl = 0;
	}
	wbcnt = 0;
	cerra << endl;
}

void halt()
{
	cerr << "simulation halt" << endl;
	fstat << "cpuclk:" << cpuclk << " IPC:" << (double)inscnt / (double)cpuclk << endl;
	fstat << "BP type:" << BPdesc[BPno] << endl;
	fstat << "ctrl stalls:" << ctrl_stall_cnt << '\t' << "data stalls:" << data_stall_cnt << endl;
	fstat << "BP:" << "sum:" << bp_total << " T:" << bp_ptrue << " TW:" << bp_twrong << " F:" << bp_pfalse <<  " FW:" << bp_fwrong << " misrate:" << (double)(bp_fwrong + bp_twrong) / (double)(bp_total) << endl;
}

void run(int mp)
{
	cpuclk = 0;
	long long reg[35] = {0};
	long long &pc = reg[34], &sp = reg[29];
	pc = mp;
	sp = M - (1 << 8);
	cerr << "simulation start" << endl;

	int op = 0;
	unsigned char ins[15] = {0};
	long long pd[6] = {0};
	int ret[10] = {0};
	int mret[5] = {0};
	int wbcnt = 0, wbreg[5] = {0}, wbval[5] = {0};
	int exwbc = 0, exwbr[5] = {0}, exwbv[5] = {0};
	long long rfwd[35] = {0};

	int bpnum = 0;

	while (pc)
	{
		cerra << '>' << cpuclk << " pc:" << pc << ' ' << insstr[data[pc] - 1] << '{' << endl;
#if PIPE
		cerra << "hz:" << hz_ctl << hz_dat << endl;
		WB(wbcnt, wbreg, wbval, reg);
		MA(mret, ret);
		if (mret[0])
		{
			wbreg[wbcnt] = mret[1];
			wbval[wbcnt++] = mret[2];
			hz_dat = 0;
		}
		for (int i = 0; i < exwbc; ++i)
		{
			wbreg[wbcnt] = exwbr[i];
			wbval[wbcnt++] = exwbv[i];
		}
		exwbc = 0;
		EX(ret, op, pd);
		if (ret[0] == 1)
		{
			exwbr[exwbc] = ret[2];
			exwbv[exwbc++] = ret[3];
			if (ret[1] == 2)
			{
				exwbr[exwbc] = ret[4];
				exwbv[exwbc++] = ret[5];
			}
			if (bpnum) BPFB(bpnum, (ret[0] == 1 && ret[2] == 34)), bpnum = 0;
			if (purge)
			{
				cerra << "####PURGING PIPE PIPE STALL" << endl;
				op = 0;
				ins[0] = 0;
				pd[0] = pd[1] = pd[2] = pd[3] = 0;

				purge = 0;
				hz_ctl = 1;
			}
		}
		else if (ret[0] == 2)
		{
			++data_stall_cnt;
			hz_dat = 1;
			cerra << "####DATA HAZARD PIPE STALL" << endl;
		}
		if (hz_dat) continue;
		//operands fwd
		memcpy(rfwd, reg, 35 * sizeof(long long));
		int wcnt = wbcnt;
		while(wcnt--)
		{
			cerra << "f:" << wbreg[wcnt] << ' ' << wbval[wcnt] << ' ';
			rfwd[wbreg[wcnt]] = wbval[wcnt];
		}
		wcnt = exwbc;
		while(wcnt--)
		{
			cerra << "f:" << exwbr[wcnt] << ' ' << exwbv[wcnt] << ' ';
			rfwd[exwbr[wcnt]] = exwbv[wcnt];
		}
		cerra << endl;
		ID(op, pd, ins, rfwd);

		if ((op > 32 && op < 50) || (op == 51 && rfwd[2] > 9))
		{
			if (op == 33 || op > 45)
			{
				//stall
				++ctrl_stall_cnt;
				hz_ctl = 1;
				cerra << "####CTRL HAZARD PIPE STALL" << endl;
			}
			else
			{
				//branch predict
				bpnum = pc - 12;
				int t = BP(bpnum, pd[2]);
				++bp_total;
				if (t)
				{
					++ctrl_stall_cnt;
					++bp_ptrue;
					hz_ctl = 1;
					cerra << "####BRANCH PREDICT TRUE PIPE STALL" << endl;
				}
				else
				{
					++bp_pfalse;
					cerra << "####BRANCH PREDICT FALSE" << endl;
				}
			}
		}

		IF(ins, pc);

#else
		IF(ins, pc);
		ID(op, pd, ins, reg);
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
#endif
		cerra << '}' << endl;
		++cpuclk;
	}
	halt();
}

//};
#endif

