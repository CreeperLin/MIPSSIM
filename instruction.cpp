#ifndef _INST_H
#define _INST_H
#include <iostream>
#include "mipsim.hpp"
#define GOTO(x) ret[1] = ret[0] = 1,ret[2] = 34, ret[3] = x;
void ADD(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1] + pd[2];
}
void ADDU(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (unsigned)(pd[1] + pd[2]);
}
void ADDIU(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (unsigned)(pd[1] + pd[2]);
}
void SUB(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1] - pd[2];
}
void SUBU(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (unsigned)(pd[1] - pd[2]);
}
void MUL(int *ret, long long *pd)
{
	ret[0] = 1;
	if (pd[4])
	{
		ret[1] = 1;
		ret[2] = pd[3];
		ret[3] = pd[1] * pd[2];
	}
	else
	{
		ret[1] = 2;
		unsigned long long t = pd[0] * pd[1];
		ret[2] = 33;
		ret[3] = t >> 32;
		ret[4] = 32;
		ret[5] = t & ((1LL << 32) - 1);
	}
}
void MULU(int *ret, long long *pd)
{
	ret[0] = 1;
	if (pd[4])
	{
		ret[1] = 1;
		ret[2] = pd[3];
		ret[3] = (unsigned)(pd[1] * pd[2]);
	}
	else
	{
		unsigned long long t = pd[0] * pd[1];
		ret[1] = 2;
		ret[2] = 33;
		ret[3] = t >> 32;
		ret[4] = 32;
		ret[5] = t & ((1LL << 32) - 1);
	}
}
void DIVU(int *ret, long long *pd)
{
	ret[0] = 1;
	if (pd[4])
	{
		ret[1] = 1;
		ret[2] = pd[3];
		ret[3] = (unsigned)(pd[1] / pd[2]);
	}
	else
	{
		ret[1] = 2;
		ret[2] = 33;
		ret[3] = (unsigned)(pd[0] % pd[1]);
		ret[4] = 32;
		ret[5] = (unsigned)(pd[0] / pd[1]);
	}
}
void DIV(int *ret, long long *pd)
{
	ret[0] = 1;
	if (pd[4])
	{
		ret[1] = 1;
		ret[2] = pd[3];
		ret[3] = pd[1] / pd[2];
	}
	else
	{
		ret[1] = 2;
		ret[2] = 33;
		ret[3] = pd[0] % pd[1];
		ret[4] = 32;
		ret[5] = pd[0] / pd[1];
	}
}
void XOR(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1] ^ pd[2];
}
void XORU(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (unsigned)(pd[1] ^ pd[2]);
}
void NEG(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = -pd[1];
}
void NEGU(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (unsigned)(-pd[1]);
}
void REM(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1] % pd[2];
}
void REMU(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (unsigned)(pd[1] % pd[2]);
}
void LI(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1];
}
void SEQ(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (pd[1] == pd[2]);
}
void SGE(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (pd[1] >= pd[2]);
}
void SGT(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (pd[1] > pd[2]);
}
void SLE(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (pd[1] <= pd[2]);
}
void SLT(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (pd[1] < pd[2]);
}
void SNE(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = (pd[1] != pd[2]);
}
void B(int *ret, long long *pd)
{
	GOTO(pd[2]);
}
void BEQ(int *ret, long long *pd)
{
	if (pd[0] == pd[1]) GOTO(pd[2]);
}
void BNE(int *ret, long long *pd)
{
	if (pd[0] != pd[1]) GOTO(pd[2]);
}
void BGE(int *ret, long long *pd)
{
	if (pd[0] >= pd[1]) GOTO(pd[2]);
}
void BLE(int *ret, long long *pd)
{
	if (pd[0] <= pd[1]) GOTO(pd[2]);
}
void BGT(int *ret, long long *pd)
{
	if (pd[0] > pd[1]) GOTO(pd[2]);
}
void BLT(int *ret, long long *pd)
{
	if (pd[0] < pd[1]) GOTO(pd[2]);
}
void BEQZ(int *ret, long long *pd)
{
	if (pd[0] == 0) GOTO(pd[2]);
}
void BNEZ(int *ret, long long *pd)
{
	if (pd[0] != 0) GOTO(pd[2]);
}
void BGEZ(int *ret, long long *pd)
{
	if (pd[0] >= 0) GOTO(pd[2]);
}
void BLEZ(int *ret, long long *pd)
{
	if (pd[0] <= 0) GOTO(pd[2]);
}
void BGTZ(int *ret, long long *pd)
{
	if (pd[0] > 0) GOTO(pd[2]);
}
void BLTZ(int *ret, long long *pd)
{
	if (pd[0] < 0) GOTO(pd[2]);
}
void J(int *ret, long long *pd)
{
	GOTO(pd[2]);
}
void JR(int *ret, long long *pd)
{
	GOTO(pd[0]);
}
void JAL(int *ret, long long *pd)
{
	ret[0] = 1;
	ret[1] = 2;
	ret[2] = 31;
	ret[3] = pd[1];
	ret[4] = 34;
	ret[5] = pd[2];
}
void JALR(int *ret, long long *pd)
{
	ret[0] = 1;
	ret[1] = 2;
	ret[2] = 31;
	ret[3] = pd[1];
	ret[4] = 34;
	ret[5] = pd[0];
}
void LA(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[2] + pd[1];
}
void LB(int *ret, long long *pd)
{
	ret[0] = 2;
	ret[2] = pd[2] + pd[1];
	ret[3] = pd[3];
	ret[4] = sizeof(byte);
}
void LH(int *ret, long long *pd)
{
	ret[0] = 2;
	ret[2] = pd[2] + pd[1];
	ret[3] = pd[3];
	ret[4] = sizeof(half);
}
void LW(int *ret, long long *pd)
{
	ret[0] = 2;
	ret[2] = pd[2] + pd[1];
	ret[3] = pd[3];
	ret[4] = sizeof(word);
}
void SB(int *ret, long long *pd)
{
	ret[0] = 3;
	ret[2] = pd[2] + pd[1];
	ret[3] = pd[0];
	ret[4] = sizeof(byte);
}
void SH(int *ret, long long *pd)
{
	ret[0] = 3;
	ret[2] = pd[2] + pd[1];
	ret[3] = pd[0];
	ret[4] = sizeof(half);
}
void SW(int *ret, long long *pd)
{
	ret[0] = 3;
	ret[2] = pd[2] + pd[1];
	ret[3] = pd[0];
	ret[4] = sizeof(word);
}
void MOVE(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1];
}
void MFHI(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1];
}
void MFLO(int *ret, long long *pd)
{
	ret[1] = ret[0] = 1;
	ret[2] = pd[3];
	ret[3] = pd[1];
}
void NOP(int *ret, long long *pd)
{
	return;
}
void SYSCALL(int *ret, long long *pd)
{
	switch (pd[0])
	{
		case 1:
		{
			cerr << "###SIMOUT:" << pd[1] << endl;
			fout << pd[1];
			break;
		}
		case 4:
		{
			cerr << "###SIMOUT:" << (data + pd[1]) << endl;
			fout << (data + pd[1]);
			break;
		}
		case 5:
		{
			ret[1] = ret[0] = 1;
			ret[2] = 2;
			fin >> ret[3];
			cerr << "###SIMIN:" << ret[3] << endl;
			break;
		}
		case 8:
		{
//			int i = 0;
//			char ch = 0;
//			cin.get(data + pd[1], pd[2]);
//			string str;
//			fin >> str;
//			str.substr(0, pd[2] - 1);
//			memcpy(data+pd[1],str.c_str(),str.length());
			fin >> (data + pd[1]);
//			for(; !cin.eof(int *ret, long long *pd)&&; ++i)
//			{
//				cin.get(ch);
//				if (ch == '\n') break;
//				data[pd[1] + i] = ch;
//			}
//			data[pd[1] + i] = '\0';
			cerr << "###SIMGETSTR:" << (data + pd[1]) << endl;
			break;
		}
		case 9:
		{
			cerr << "###SIMALLOCATE" << endl;
			int np = data_p + 100, t = 1 << 2, s = t;
			while (s < np && s < M) s += t;
			data_p = s + pd[1];
			ret[1] = ret[0] = 1;
			ret[2] = 2;
			ret[3] = s;
			break;
		}
		case 10:
			cerr << "###SIMTERMINATE" << endl;
			ret[1] = ret[0] = 1;
			ret[2] = 34;
			ret[3] = 0;
			break;
		case 17:
			cerr << "###SIMTERMINATE:" << pd[1] << endl;
			ret[1] = ret[0] = 1;
			ret[2] = 34;
			ret[3] = 0;
			break;
		default:
			cerr << "!sys" << endl;
			exit(2);
	}
}
#endif
