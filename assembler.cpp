#include <bits/stdc++.h>
#include "main.h"
//#include <iostream>
using namespace std;
const int TEXT = 0x500;
const int DATA = 0x15000;
const byte JI = 1 << 7 + 1 << 6;
const byte II = 1 << 7;
const byte RI = 1 << 6;
//const byte OI = 0;
int text_p;
int data_p;
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
void init()
{
	opmap["0"] = 0;
	opmap["1"] = 1;
	opmap["2"] = 2;
	opmap["3"] = 3;
	opmap["4"] = 4;
	opmap["5"] = 5;
	opmap["6"] = 6;
	opmap["7"] = 7;
	opmap["8"] = 8;
	opmap["9"] = 9;
	opmap["10"] = 10;
	opmap["11"] = 11;
	opmap["12"] = 12;
	opmap["13"] = 13;
	opmap["14"] = 14;
	opmap["15"] = 15;
	opmap["16"] = 16;
	opmap["17"] = 17;
	opmap["18"] = 18;
	opmap["19"] = 19;
	opmap["20"] = 20;
	opmap["21"] = 21;
	opmap["22"] = 22;
	opmap["23"] = 23;
	opmap["24"] = 24;
	opmap["25"] = 25;
	opmap["26"] = 26;
	opmap["27"] = 27;
	opmap["28"] = 28;
	opmap["29"] = 29;
	opmap["30"] = 30;
	opmap["31"] = 31;

	opmap["zero"] = 0;
	opmap["at"] = 1;
	opmap["v0"] = 2;
	opmap["v1"] = 3;
	opmap["a0"] = 4;
	opmap["a1"] = 5;
	opmap["a2"] = 6;
	opmap["a3"] = 7;
	opmap["t0"] = 8;
	opmap["t1"] = 9;
	opmap["t2"] = 10;
	opmap["t3"] = 11;
	opmap["t4"] = 12;
	opmap["t5"] = 13;
	opmap["t6"] = 14;
	opmap["t7"] = 15;
	opmap["t8"] = 16;
	opmap["t9"] = 17;
	opmap["s0"] = 18;
	opmap["s1"] = 19;
	opmap["s2"] = 20;
	opmap["s3"] = 21;
	opmap["s4"] = 22;
	opmap["s5"] = 23;
	opmap["s6"] = 24;
	opmap["s7"] = 25;
	opmap["k0"] = 26;
	opmap["k1"] = 27;
	opmap["gp"] = 28;
	opmap["sp"] = 29;
	opmap["s8"] = 30;
	opmap["fp"] = 30;
	opmap["ra"] = 31;

	opmap["align"] = 1;
	opmap["ascii"] = 2; 
	opmap["asciiz"] = 3;
	opmap["byte"] = 4;
	opmap["half"] = 5;
	opmap["word"] = 6;
	opmap["space"] = 7;
	opmap["data"] = 8;
	opmap["text"] = 9;

	opmap["add"] = 1;
	opmap["addu"] = 2;
	opmap["addiu"] = 3;
	opmap["sub"] = 4;
	opmap["subu"] = 5;
	opmap["mul"] = 6;
	opmap["mulu"] = 7;
	opmap["div"] = 8;
	opmap["divu"] = 9;
	opmap["xor"] = 10;
	opmap["xoru"] = 11;
	opmap["neg"] = 12;
	opmap["negu"] = 13;
	opmap["rem"] = 14;
	opmap["remu"] = 15;
	opmap["li"] = 16;
	opmap["seq"] = 17;
	opmap["sge"] = 18;
	opmap["sgt"] = 19;
	opmap["sle"] = 20;
	opmap["slt"] = 21;
	opmap["sne"] = 22;
	opmap["b"] = 23;
	opmap["beq"] = 24;
	opmap["bne"] = 25;
	opmap["bge"] = 26;
	opmap["ble"] = 27;
	opmap["bgt"] = 28;
	opmap["blt"] = 29;
	opmap["beqz"] = 30;
	opmap["bnez"] = 31;
	opmap["blez"] = 32;
	opmap["bgez"] = 33;
	opmap["bgtz"] = 34;
	opmap["bltz"] = 35;
	opmap["j"] = 36;
	opmap["jr"] = 37;
	opmap["jal"] = 38;
	opmap["jalr"] = 39;
	opmap["la"] = 40;
	opmap["lb"] = 41;
	opmap["lh"] = 42;
	opmap["lw"] = 43;
	opmap["sb"] = 44;
	opmap["sh"] = 45;
	opmap["sw"] = 46;
	opmap["move"] = 47;
	opmap["mfhi"] = 48;
	opmap["mflo"] = 49;
	opmap["nop"] = 50;
	opmap["syscall"] = 51;
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
void eva(int &p)
{
	cout << '@' << p << ' ' << lex[p] << endl;
	int &cp = data_p;
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
					int t = 1;
					for (int i = 0; i < n; ++i) t <<= 1;
					int s = t;
					while (s < p && s < M) s += t;
					cp = s;
					break;
				}
				case 2://.ascii
				{
					string &v = lex[++p].sval;
					for (int i = 0; i < v.length(); ++i)
					{
						data[cp++] = v[i];
					}
					break;
				}
				case 3://.asciiz
				{
					string &v = lex[++p].sval;
					for (int i = 0; i < v.length(); ++i)
					{
						data[cp++] = v[i];
					}
					data[cp++] = '\0';
					break;
				}
				case 4://.byte
				{
					while (lex[++p].type == numt)
					{
						data[cp++] = (byte)lex[p].nval;
					}
					--p;
					break;
				}
				case 5://.half
				{
					do
					{
						memcpy(data + cp, &lex[++p].nval, sizeof(half));
						cp += sizeof(half);
					}while (lex[p+1].type == numt);
					break;
				}
				case 6://.word
				{
					while (lex[++p].type == numt)
					{
						memcpy(data + cp, &lex[p].nval, sizeof(word));
						cp += sizeof(word);
					}
					--p;
					break;
				}
				case 7://.space
				{
					int n = lex[++p].nval;
					assert(lex[p].type == numt);
					memset(data + cp, 0, n * sizeof(byte));
					break;
				}
				case 8://.data
					cp = data_p;
					break;
				case 9://.text
					cp = text_p;
					break;
				default:
					cout << "!eva asst " << lex[p] << endl;
			}
			++p;
			break;
		}
		case opct:
		{
			assert(cp == text_p);
			data[cp] = opmap[lex[p].sval];
			int cnt = 0, rcnt = 0;
			while (lex[++p].type != opct)
			{
				switch (lex[p].type)
				{
					case numt:
					{
						data[cp + 2 + cnt * 3] = 0;
						memcpy(data + cp + 3 + cnt * 3, &lex[p].nval, sizeof(half));
						++cnt;
						break;
					}
					case regt:
					{
						if (rcnt)
						{
							data[cp + 2 + cnt * 3] = 1;
							data[cp + 3 + cnt * 3] = opmap[lex[p].sval];
							++cnt;
						}
						else
						{
							data[cp + 1] = opmap[lex[p].sval];
							++rcnt;
						}
						break;
					}
					case labt:
					{
						memcpy(data + cp + 4, &lbmap[lex[p].sval], sizeof(word));
						break;
					}
					case addt:
					{
						memcpy(data + cp + 6, &lex[p].nval, sizeof(half));
						data[cp + 2] = 1;
						data[cp + 3] = lbmap[lex[p].sval];
						break;
					}
					default:
						cout << "!eva opt:" << lex[p] << endl;
				}
			}
			cp += 8;
			break;
		}
		case labt:
		{
			lbmap[lex[p++].sval] = cp;
			break;
		}
		default:
			cout << "!eva:" << lex[p] << endl;
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
	cout << "evaluation begin" << endl;
	while (pos < s)
	{
		eva(pos);
	}
}
