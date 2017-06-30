#include <bits/stdc++.h>
#include "main.h"
#include "instruction.cpp"
//#include <iostream>
using namespace std;
const int TEXT = 0x500;
const int DATA = 0x15000;
const byte RI = 1 << 7;
const byte JI = 1 << 6;
//const byte OI = 0;
int text_p = TEXT;
int data_p = DATA;
enum tid
{
    asst, opct, regt, numt, litt, addt, labt
};
struct token
{
	tid type;
	string sval;
	int nval;
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
		}
		os << ' ' << t.nval << ' ' << t.sval;
		return os;
	}
};
unordered_map<string, byte> opmap;
unordered_map<string, int> lbmap;
vector<token> lex;
char *numstr[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30", "31"};
char *regstr[] = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9", "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "k0", "k1", "gp", "sp", "s8", "ra", "lo", "hi", "pc"};
char *assstr[] = {"align", "ascii", "asciiz", "byte", "half", "word", "space", "data", "text"};
char *insstr[] = {"add", "addu", "addiu", "sub", "subu", "mul", "mulu", "div", "divu", "xor", "xoru", "neg", "negu", "rem", "remu", "li", "seq", "sge", "sgt", "sle", "slt", "sne", "b", "beq", "bne", "bge", "ble", "bgt", "blt", "beqz", "bnez", "blez", "bgez", "bgtz", "bltz", "j", "jr", "jal", "jalr", "la", "lb", "lh", "lw", "sb", "sh", "sw", "move", "mfhi", "mflo", "nop", "syscall"};
void init()
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
	insp[23] = B;
	insp[24] = BEQ;
	insp[25] = BNE;
	insp[26] = BGE;
	insp[27] = BLE;
	insp[28] = BGT;
	insp[29] = BLT;
	insp[30] = BEQZ;
	insp[31] = BNEZ;
	insp[32] = BLEZ;
	insp[33] = BGEZ;
	insp[34] = BGTZ;
	insp[35] = BLTZ;
	insp[36] = J;
	insp[37] = JR;
	insp[38] = JAL;
	insp[39] = JALR;
	insp[40] = LA;
	insp[41] = LB;
	insp[42] = LH;
	insp[43] = LW;
	insp[44] = SB;
	insp[45] = SH;
	insp[46] = SW;
	insp[47] = MOVE;
	insp[48] = MFHI;
	insp[49] = MFLO;
	insp[50] = NOP;
	insp[51] = SYSCALL;

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
				if (typ != litt) f = 0;
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
						cout << "!esc char.";
				}
				break;
			}
			case ':':
				f = 0;
				ret.type = labt;
				break;
			default:
				v += str[p];
		}
		++p;
	}
	if (typ == opct && !opmap[v]) ret.type = labt;
	cout << "gettxt:" << ret << endl;
	return ret;
}
token getnum(string &str, int &p, tid typ = numt)
{
	token ret;
	ret.type = typ;
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
			cout << "addr" << endl;
			return ret;
		}
		ret.type = addt;
		while (str[++p] != ')')
		{
			ret.sval += str[p];
		}
		++p;
	}
	cout << "getnum:" << ret << endl;
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
				lex.push_back(gettxt(str, ++p, litt));
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
int *cp;
void eva(int &p)
{
	cout << '@' << p << ' ' << lex[p] << endl;
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
					int t = 1 << n, s = t;
					while (s < *cp && s < M) s += t;
					*cp = s;
					break;
				}
				case 2://.ascii
				{
					string &v = lex[++p].sval;
					for (int i = 0; i < v.length(); ++i)
					{
						data[(*cp)++] = v[i];
					}
					break;
				}
				case 3://.asciiz
				{
					string &v = lex[++p].sval;
					for (int i = 0; i < v.length(); ++i)
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
//					cout << "$.data:" << *cp << ' ' << data_p << endl;
					break;
				case 9://.text
					cp = &text_p;
//					cout << "$.text:" << *cp << ' ' << text_p << endl;
					break;
				default:
					cout << "!eva asst " << lex[p] << endl;
			}
			++p;
			cout << '%' << data_p << ' ' << text_p << endl;
			break;
		}
		case opct:
		{
//			cout<<"$opct:"<<*cp<<' '<<text_p<<endl;
			assert(cp == &text_p);
			data[*cp] = opmap[lex[p].sval];
			int cnt = 0, rcnt = 0, f = 1;
			while (f)
			{
				switch (lex[++p].type)
				{
					case opct:
					case asst:
						f = 0;
						break;
					case numt:
					{
						data[*cp + 2 + cnt * 3] = 1;
						memcpy(data + *cp + 3 + cnt * 3, &lex[p].nval, sizeof(half));
						++cnt;
						break;
					}
					case regt:
					{
						if (rcnt)
						{
							data[*cp + 2 + cnt * 3] = 2;
							data[*cp + 3 + cnt * 3] = opmap[lex[p].sval];
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
						memcpy(data + *cp + 4, &lbmap[lex[p].sval], sizeof(word));
						break;
					}
					case addt:
					{
						data[*cp + 5] = 1;
						memcpy(data + *cp + 6, &lex[p].nval, sizeof(half));
						data[*cp + 2] = 2;
						data[*cp + 3] = lbmap[lex[p].sval];
						break;
					}
					default:
						cout << "!eva opt:" << lex[p] << endl;
				}
			}
			(*cp) += 8;
			break;
		}
		case labt:
		{
			lbmap[lex[p++].sval] = *cp;
			break;
		}
		default:
			cout << "!eva:" << lex[p] << endl;
			exit(0);
	}
}
void compile()
{
	string str;
	int i = 1;
	while (!fas.eof())
	{
		getline(fas, str);
		cout << '#' << i++ << ' ' << str << endl;
		appexpr(str);
	}
	int pos = 0, s = lex.size();
	cout << "lex:" << s << endl;
	for (int i = 0; i < s; ++i)
		cout << i << ':' << lex[i] << endl;
	cout << "eva begin" << endl;
	while (pos < s)
	{
		eva(pos);
	}
}
