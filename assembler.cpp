#include <bits/stdc++.h>
#include "main.h"
#include "instruction.cpp"
using namespace std;

int *cp;
int lbidx[100000], lblex[100000], lbcnt;
class MIPSAssembler
{
unordered_map<string, byte> opmap;
unordered_map<string, int> lbmap;
vector<token> lex;
	
enum tid
{
    asst, opct, regt, numt, litt, addt, labt, lbdt
};
struct token
{
	tid type;
	int nval;
	string sval;
	token(): type(asst), nval(0), sval() {}
	token(tid _t, int _n): type(_t), nval(_n), sval() {}
	friend ostream& operator<<(ostream &os, token &t)
	{
		switch (t.type)
		{
			case asst:
				os << "asst";
				break;
			case opct:
				os << "opct";
				break;
			case regt:
				os << "regt";
				break;
			case numt:
				os << "numt";
				break;
			case litt:
				os << "litt";
				break;
			case addt:
				os << "addt";
				break;
			case labt:
				os << "labt";
				break;
			case lbdt:
				os << "lbdt";
				break;
		}
		os << ' ' << t.nval << ' ' << t.sval;
		return os;
	}
};

MIPSAssembler()
{
	for (int i = 0; i < 32; ++i)
	{
		opmap[numstr[i]] = i;
	}
	for (int i = 0; i < 35; ++i)
	{
		opmap[regstr[i]] = i;
	}
	opmap["fp"] = 30;
	for (int i = 0; i < 9; ++i)
	{
		opmap[assstr[i]] = i + 1;
	}
	for (int i = 0; i < 51; ++i)
	{
		opmap[insstr[i]] = i + 1;
	}

	insp[1] = ADD;
	insp[2] = ADDU;
	insp[3] = ADDIU;
	insp[4] = SUB;
	insp[5] = SUBU;
	insp[6] = MUL;
	insp[7] = MULU;
	insp[8] = DIV;
	insp[9] = DIVU;
	insp[10] = XOR;
	insp[11] = XORU;
	insp[12] = NEG;
	insp[13] = NEGU;
	insp[14] = REM;
	insp[15] = REMU;
	insp[16] = LI;
	insp[17] = SEQ;
	insp[18] = SGE;
	insp[19] = SGT;
	insp[20] = SLE;
	insp[21] = SLT;
	insp[22] = SNE;
	insp[23] = MOVE;
	insp[24] = MFHI;
	insp[25] = MFLO;

	insp[26] = LA;
	insp[27] = LB;
	insp[28] = LH;
	insp[29] = LW;
	insp[30] = SB;
	insp[31] = SH;
	insp[32] = SW;

	insp[33] = B;
	insp[34] = BEQ;
	insp[35] = BNE;
	insp[36] = BGE;
	insp[37] = BLE;
	insp[38] = BGT;
	insp[39] = BLT;
	insp[40] = BEQZ;
	insp[41] = BNEZ;
	insp[42] = BLEZ;
	insp[43] = BGEZ;
	insp[44] = BGTZ;
	insp[45] = BLTZ;
	insp[46] = J;
	insp[47] = JR;
	insp[48] = JAL;
	insp[49] = JALR;

	insp[50] = NOP;
	insp[51] = SYSCALL;
}
token getlit(string &str, int &p)
{
	token ret;
	ret.type = litt;
	string &v = ret.sval;
	int len = str.length(), f = 1;
	while(p < len && f)
	{
		switch (str[p])
		{
			case '\"':
				f = 0;
				break;
			case '\\':
			{
				switch(str[++p])
				{
					case 'n':
						v += '\n';
						break;
					case 't':
						v += '\t';
						break;
					case '\\':
					case '\'':
					case '\"':
						v += str[p];
						break;
					default:
						cerr << "!esc char.";
						exit(1);
				}
				break;
			}
			default:
				v += str[p];
		}
		++p;
	}
	return ret;
}
token gettxt(string &str, int &p, tid typ)
{
	token ret;
	ret.type = typ;
	string &v = ret.sval;
	int len = str.length(), f = 1;
	while(p < len && f)
	{
		switch (str[p])
		{
			case ' ':
			case '\t':
			case '\n':
			case ',':
				f = 0;
				break;
			case '\"':
				f = 0;
				break;
			case '\\':
			{
				switch(str[++p])
				{
					case 'n':
						v += '\n';
						break;
					case 't':
						v += '\t';
						break;
					case '\\':
					case '\'':
					case '\"':
						v += str[p];
						break;
					default:
						cerr << "!esc char.";
						exit(1);
				}
				break;
			}
			case ':':
				f = 0;
				ret.type = lbdt;
				break;
			default:
				v += str[p];
		}
		++p;
	}
	if (typ == opct && ret.type != lbdt && !opmap[v]) ret.type = labt;
	return ret;
}
token getnum(string &str, int &p)
{
	token ret;
	ret.type = numt;
	int s = 1, len = str.length(), &v = ret.nval;
	if (str[p] == '-') s = -1, ++p;
	int t = p;
	while (p < len && str[p] >= '0' && str[p] <= '9') ++p;
	for (int i = p - 1, j = 1; i >= t; --i, j *= 10)
	{
		v += j * (str[i] - '0');
	}
	v *= s;
	if (str[p] == '(')
	{
		if (str[++p] != '$')
		{
			return ret;
		}
		ret.type = addt;
		while (str[++p] != ')')
		{
			ret.sval += str[p];
		}
		++p;
	}
	return ret;
}
void appexpr(string &str)
{
	int p = 0, len = str.length();
	while(p < len)
	{
		switch(str[p])
		{
			case ' ':
			case '\t':
			case ',':
				++p;
				break;
			case '.':
				lex.push_back(gettxt(str, ++p, asst));
				break;
			case '-':
			case '+':
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				lex.push_back(getnum(str, p));
				break;
			case '$':
				lex.push_back(gettxt(str, ++p, regt));
				break;
			case '\"':
				lex.push_back(getlit(str, ++p));
				break;
			case '\'':
				lex.push_back(token(numt, (int)str[++p]));
				assert(str[p + 1] == '\'');
				p += 2;
				break;
			case '#':
				return;
			default:
				lex.push_back(gettxt(str, p, opct));
		}
	}
}
void eva(unsigned int &p)
{
	if (p > lex.size()) return;
	switch (lex[p].type)
	{
		case asst:
		{
			switch(opmap[lex[p].sval])
			{
				case 1://.align
				{
					int n = lex[++p].nval;
					assert(lex[p].type == numt);
					int t = 1 << n;
					*cp = ((*cp - 1) / t + 1) * t;
					break;
				}
				case 2://.ascii
				{
					string &v = lex[++p].sval;
					for (unsigned int i = 0; i < v.length(); ++i)
					{
						data[(*cp)++] = v[i];
					}
					break;
				}
				case 3://.asciiz
				{
					string &v = lex[++p].sval;
					for (unsigned int i = 0; i < v.length(); ++i)
					{
						data[(*cp)++] = v[i];
					}
					data[(*cp)++] = '\0';
					break;
				}
				case 4://.byte
				{
					while (lex[++p].type == numt)
					{
						data[(*cp)++] = (byte)lex[p].nval;
					}
					--p;
					break;
				}
				case 5://.half
				{
					do
					{
						memcpy(data + *cp, &lex[++p].nval, sizeof(half));
						*cp += sizeof(half);
					}
					while (lex[p + 1].type == numt);
					break;
				}
				case 6://.word
				{
					while (lex[++p].type == numt)
					{
						memcpy(data + *cp, &lex[p].nval, sizeof(word));
						*cp += sizeof(word);
					}
					--p;
					break;
				}
				case 7://.space
				{
					int n = lex[++p].nval;
					assert(lex[p].type == numt);
					memset(data + *cp, 0, n * sizeof(byte));
					break;
				}
				case 8://.data
					cp = &data_p;
					break;
				case 9://.text
					cp = &text_p;
					break;
				default:
					exit(1);
			}
			++p;
			break;
		}
		case opct:
		{
			assert(cp == &text_p);
			data[*cp] = opmap[lex[p].sval];
			int cnt = 0, rcnt = 0, f = 1;
			while (f)
			{
				switch (lex[++p].type)
				{
					case opct:
					case asst:
					case lbdt:
						f = 0;
						break;
					case numt:
					{
						data[*cp + 2 + cnt * 5] = 1;
						memcpy(data + *cp + 3 + cnt * 5, &lex[p].nval, sizeof(word));
						++cnt;
						break;
					}
					case regt:
					{
						if (rcnt)
						{
							data[*cp + 2 + cnt * 5] = 2;
							data[*cp + 3 + cnt * 5] = opmap[lex[p].sval];
							++cnt;
						}
						else
						{
							data[*cp + 1] = opmap[lex[p].sval];
							++rcnt;
						}
						break;
					}
					case labt:
					{
						if (lbmap[lex[p].sval])
						{
							memcpy(data + *cp + 8, &lbmap[lex[p].sval], sizeof(word));
						}
						else
						{
							lbidx[lbcnt] = *cp;
							lblex[lbcnt++] = p;
						}
						break;
					}
					case addt:
					{
						data[*cp + 7] = 1;
						memcpy(data + *cp + 8, &lex[p].nval, sizeof(word));
						data[*cp + 2] = 2;
						data[*cp + 3] = opmap[lex[p].sval];
						break;
					}
					default:
						exit(1);
				}
			}
			(*cp) += 12;
			break;
		}
		case lbdt:
		{
			lbmap[lex[p++].sval] = *cp;
			break;
		}
		default:
			exit(1);
	}
}
int compile()
{
	string str;
	while (!fas.eof())
	{
		getline(fas, str);
		appexpr(str);
	}
	unsigned int pos = 0, s = lex.size();
	while (pos < s)
	{
		eva(pos);
	}
	for (int i = 0; i < lbcnt; ++i)
	{
		assert(lbmap[lex[lblex[i]].sval] != 0);
		memcpy(data + lbidx[i] + 8, &lbmap[lex[lblex[i]].sval], sizeof(word));
	}
	return lbmap["main"];
}
};
