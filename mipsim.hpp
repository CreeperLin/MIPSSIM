#ifndef MIPSSIM_H
#define MIPSSIM_H
#include "main.h"
#include "instruction.hpp"
#include "branch_predict.hpp"
using namespace std;
int hz_ctl = 0, hz_dat = 0, purge = 0;
class MIPSIM
{
	long long Reg[35];
	void IF(unsigned char *ins, long long &pc)
	{
		if (hz_ctl || !pc)	return;
		memcpy(ins, data + pc, 12);
		pc += 12;
	}

	void ID(int &op, long long *pd, unsigned char* ins, long long *reg)
	{
		op = ins[0];
		ins[0] = 0;
		if (!op)	return;
		if (op < 24)
		{
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
			pd[0] = reg[ins[1]];
			pd[1] = (ins[2] == 2) ? reg[ins[3]] : 0;
			pd[2] = *reinterpret_cast<word*>(ins + 8);
			pd[3] = ins[1];
		}
		else if (op < 50)
		{
			pd[0] = reg[ins[1]];
			pd[1] = (ins[2] == 1) ? (*reinterpret_cast<word*>(ins + 3)) : ((ins[2] == 2) ? reg[ins[3]] : reg[34]);
			pd[2] = *reinterpret_cast<word*>(ins + 8);
		}
		else if (op < 52)
		{
			if (op == 51)
			{
				pd[0] = reg[2];
				pd[1] = reg[4];
				pd[2] = reg[5];
			}
		}
		else
		{
			exit(2);
		}
	}

	void EX(int *ret, int &op, long long *pa)
	{
		ret[0] = ret[1] = 0;
		if (op)
		{
			insp[op](ret, pa);
			op = 0;
			if (hz_ctl && !ret[1])
			{
				hz_ctl = 0;
			}
			else if(!hz_ctl && ret[0] == 1 && ret[2] == 34)
			{
				purge = 1;
			}
		}
	}

	void MA(int *ret, int *pa)
	{
		ret[0] = 0;
		if (pa[0] == 2)
		{
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
		}
		else if (pa[0] == 3)
		{
			memcpy(data + pa[2], pa + 3, pa[4]);
		}
		pa[0] = 0;
	}
	void WB(int &wbcnt, int *wbreg, int *wbval, long long *reg)
	{
		for (int i = 0; i < wbcnt; ++i)
		{
			reg[wbreg[i]] = wbval[i];
			if (wbreg[i] == 34) hz_ctl = 0;
		}
		wbcnt = 0;
	}

public:
	MIPSIM(int mp)
	{
		for (int i = 0; i < 35; ++i) Reg[i] = 0;
		Reg[34] = mp;
		Reg[29] = M - (1 << 8);
	}
	void run()
	{
		unsigned char ins[15] = {0};
		long long &pc = Reg[34];
		long long pd[6] = {0}, rfwd[35] = {0};
		int ret[10] = {0}, mret[5] = {0}, wbcnt = 0, wbreg[5] = {0}, wbval[5] = {0}, exwbc = 0, exwbr[5] = {0}, exwbv[5] = {0}, bpnum = 0, op = 0;
		while (pc)
		{
			WB(wbcnt, wbreg, wbval, Reg);
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
					op = 0;
					ins[0] = 0;
					pd[0] = pd[1] = pd[2] = pd[3] = 0;

					purge = 0;
					hz_ctl = 1;
				}
			}
			else if (ret[0] == 2)
			{
				hz_dat = 1;
			}
			if (hz_dat) continue;
			//operands fwd
			memcpy(rfwd, Reg, 35 * sizeof(long long));
			int wcnt = wbcnt;
			while(wcnt--)
			{
				rfwd[wbreg[wcnt]] = wbval[wcnt];
			}
			wcnt = exwbc;
			while(wcnt--)
			{
				rfwd[exwbr[wcnt]] = exwbv[wcnt];
			}
			ID(op, pd, ins, rfwd);

			if ((op > 32 && op < 50) || (op == 51 && rfwd[2] > 9))
			{
				if (op == 33 || op > 45)
				{
					//stall
					hz_ctl = 1;
				}
				else
				{
					//branch predict
					bpnum = pc - 12;
					if (BP(bpnum, pd[2]))
					{
						hz_ctl = 1;
					}
				}
			}
			IF(ins, pc);
		}
	}

};
#endif

