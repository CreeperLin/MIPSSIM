#ifndef _INST_H
#define _INST_H
#include <iostream>
#include "mipsim.hpp"
#define GOTO(x) wbreg[wbcnt]=34,wbval[wbcnt++]=x;
void ADD()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1] + pd[2];
}
void ADDU()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (unsigned)(pd[1] + pd[2]);
}
void ADDIU()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (unsigned)(pd[1] + pd[2]);
}
void SUB()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1] - pd[2];
}
void SUBU()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (unsigned)(pd[1] - pd[2]);
}
void MUL()
{
	if (pr[1])
	{
		wbreg[wbcnt] = pr[0];
		wbval[wbcnt++] = pd[1] * pd[2];
	}
	else
	{
		unsigned long long t = pd[0] * pd[1];
		wbreg[wbcnt] = 33;
		wbval[wbcnt++] = t >> 32;
		wbreg[wbcnt] = 32;
		wbval[wbcnt++] = t & ((1LL << 32) - 1);
	}
}
void MULU()
{
	if (pr[1])
	{
		wbreg[wbcnt] = pr[0];
		wbval[wbcnt++] = (unsigned)(pd[1] * pd[2]);
	}
	else
	{
		unsigned long long t = pd[0] * pd[1];
		wbreg[wbcnt] = 33;
		wbval[wbcnt++] = t >> 32;
		wbreg[wbcnt] = 32;
		wbval[wbcnt++] = t & ((1LL << 32) - 1);
	}
}
void DIVU()
{
	if (pr[1])
	{
		wbreg[wbcnt] = pr[0];
		wbval[wbcnt++] = (unsigned)(pd[1] / pd[2]);
	}
	else
	{
		wbreg[wbcnt] = 33;
		wbval[wbcnt++] = (unsigned)(pd[0] % pd[1]);
		wbreg[wbcnt] = 32;
		wbval[wbcnt++] = (unsigned)(pd[0] / pd[1]);
	}
}
void DIV()
{
	if (pr[1])
	{
		wbreg[wbcnt] = pr[0];
		wbval[wbcnt++] = pd[1] / pd[2];
	}
	else
	{
		wbreg[wbcnt] = 33;
		wbval[wbcnt++] = pd[0] % pd[1];
		wbreg[wbcnt] = 32;
		wbval[wbcnt++] = pd[0] / pd[1];
	}
}
void XOR()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1] ^ pd[2];
}
void XORU()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (unsigned)(pd[1] ^ pd[2]);
}
void NEG()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = -pd[1];
}
void NEGU()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (unsigned)(-pd[1]);
}
void REM()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1] % pd[2];
}
void REMU()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (unsigned)(pd[1] % pd[2]);
}
void LI()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1];
}
void SEQ()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (pd[1] == pd[2]);
}
void SGE()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (pd[1] >= pd[2]);
}
void SGT()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (pd[1] > pd[2]);
}
void SLE()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (pd[1] <= pd[2]);
}
void SLT()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (pd[1] < pd[2]);
}
void SNE()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = (pd[1] != pd[2]);
}
void B()
{
	GOTO(pd[2]);
}
void BEQ()
{
	if (pd[0] == pd[1]) GOTO(pd[2]);
}
void BNE()
{
	if (pd[0] != pd[1]) GOTO(pd[2]);
}
void BGE()
{
	if (pd[0] >= pd[1]) GOTO(pd[2]);
}
void BLE()
{
	if (pd[0] <= pd[1]) GOTO(pd[2]);
}
void BGT()
{
	if (pd[0] > pd[1]) GOTO(pd[2]);
}
void BLT()
{
	if (pd[0] < pd[1]) GOTO(pd[2]);
}
void BEQZ()
{
	if (pd[0] == 0) GOTO(pd[2]);
}
void BNEZ()
{
	if (pd[0] != 0) GOTO(pd[2]);
}
void BGEZ()
{
	if (pd[0] >= 0) GOTO(pd[2]);
}
void BLEZ()
{
	if (pd[0] <= 0) GOTO(pd[2]);
}
void BGTZ()
{
	if (pd[0] > 0) GOTO(pd[2]);
}
void BLTZ()
{
	if (pd[0] < 0) GOTO(pd[2]);
}
void J()
{
	GOTO(pd[2]);
}
void JR()
{
	GOTO(pd[0]);
}
void JAL()
{
	wbreg[wbcnt] = 31;
	wbval[wbcnt++] = pc;
	GOTO(pd[2]);
}
void JALR()
{
	wbreg[wbcnt] = 31;
	wbval[wbcnt++] = pc;
	GOTO(pd[2]);
}
void LA()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[2] + pd[1];
}
void LB()
{
	maflg = 1;
	maval = pr[0];
	maddr = pd[2] + pd[1];
	masiz = sizeof(byte);
}
void LH()
{
	maflg = 1;
	maval = pr[0];
	maddr = pd[2] + pd[1];
	masiz = sizeof(half);
}
void LW()
{
	maflg = 1;
	maval = pr[0];
	maddr = pd[2] + pd[1];
	masiz = sizeof(word);
}
void SB()
{
	maflg = 2;
	maddr = pd[2] + pd[1];
	maval = pd[0];
	masiz = sizeof(byte);
}
void SH()
{
	maflg = 2;
	maddr = pd[2] + pd[1];
	maval = pd[0];
	masiz = sizeof(half);
}
void SW()
{
	maflg = 2;
	maddr = pd[2] + pd[1];
	maval = pd[0];
	masiz = sizeof(word);
}
void MOVE()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1];
}
void MFHI()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1];
}
void MFLO()
{
	wbreg[wbcnt] = pr[0];
	wbval[wbcnt++] = pd[1];
}
void NOP()
{
	return;
}
void SYSCALL()
{
	switch (pd[0])
	{
		case 1:
		{
			cerr << "###SIMOUT:" << pd[1] << endl;
			cout << pd[1];
			break;
		}
		case 4:
		{
			cerr << "###SIMOUT:" << (data + pd[1]) << endl;
			cout << (data + pd[1]);
			break;
		}
		case 5:
		{
			wbreg[wbcnt] = 2;
			cin >> wbval[wbcnt++];
			cerr << "###SIMIN:" << wbval[wbcnt - 1] << endl;
			break;
		}
		case 8:
		{
			int i = 0;
			char ch = 0;
//			cin.get(data + pd[1], pd[2]);
			cin >> (data + pd[1]);
//			for(; !cin.eof()&&; ++i)
//			{
//				cin.get(ch);
//				if (ch == '\n') break;
//				data[pd[1] + i] = ch;
//			}
//			data[pd[1] + i] = '\0';
			cerr << "###SIMGETSTR:" << (data + pd[1]) << endl;
//			for (int i = pd[1];; ++i)
//			{
//				//cerr << '@' << i << ':' << data[i] << ' ';
//				if (!data[i]) break;
//			}
			//cerr << endl;
			break;
		}
		case 9:
		{
			cerr << "###SIMALLOCATE" << endl;
			int np = data_p + 100, t = 1 << 2, s = t;
			while (s < np && s < M) s += t;
			data_p = s + pd[1];
			wbreg[wbcnt] = 2;
			wbval[wbcnt++] = s;
			break;
		}
		case 10:
			cerr << "###SIMTERMINATE" << endl;
			wbreg[wbcnt] = 34;
			wbval[wbcnt++] = 0;
			break;
		case 17:
			cerr << "###SIMTERMINATE:" << pd[1] << endl;
			wbreg[wbcnt] = 34;
			wbval[wbcnt++] = 0;
			break;
		default:
			cerr << "!sys" << endl;
			exit(2);
	}
}
#endif
