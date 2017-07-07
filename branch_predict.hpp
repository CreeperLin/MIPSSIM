#ifndef _BRANCHPREDICT_
#define _BRANCHPREDICT_
#include <iostream>
using namespace std;
int BPno = 2;
string BPdesc[5] = {"static", "onebit", "TLAP", "LVQ", "perceptron"};
//perceptron
const int Neu = 1000, h = 16;
int W[Neu][1 + h], ans, G, H[100000], thershold = 12 + 2 * h;
//LVQ
const int li = 3, lk = 3, ll = 4, lt = li + lk + ll;
double Vt[lt] = {1}, Vnt[lt] = {0}, X[lt], aa = 0.01;
int HRg, HRl[100000], pdct = 0;
int LVQpredict(int bnum)
{
	int i = 0;
	double Dt = 0, Dnt = 0;
	for (int t = HRl[bnum]; i < ll; ++i)
	{
		X[i] = (t & 1);
		t >>= 1;
		Dt += (X[i] - Vt[i]) * (X[i] - Vt[i]);
		Dnt += (X[i] - Vnt[i]) * (X[i] - Vnt[i]);
	}
	for (int t = HRg; i < ll + lk; ++i)
	{
		X[i] = (t & 1);
		t >>= 1;
		Dt += (X[i] - Vt[i]) * (X[i] - Vt[i]);
		Dnt += (X[i] - Vnt[i]) * (X[i] - Vnt[i]);
	}
	for (int t = bnum; i < lt; ++i)
	{
		X[i] = (t & 1);
		t >>= 1;
		Dt += (X[i] - Vt[i]) * (X[i] - Vt[i]);
		Dnt += (X[i] - Vnt[i]) * (X[i] - Vnt[i]);
	}
//	cerr << "LVQ:" << Dt << ' ' << Dnt << endl;
	pdct = (Dt > Dnt) ? 0 : 1;
	return pdct;
}
//TLAP
const int N = 5;
struct sm
{
	int st;
	sm(): st(1) {}
	int predict()
	{
		return (st < 3) ? 0 : 1;
	}
	void change(int t)
	{
		t = (t << 1) - 1;
		st += t;
		if (st > 4) st = 4;
		else if (st < 1)st = 1;
	}
};
int Bhist[100000];
int mask = (1 << N) - 1;
sm PHT[100000][1 << N];

//one bit
int BPmap[100000];

int BP(int bnum, int tgt)
{
	switch (BPno)
	{
		case 4:
		{
			//perceptron
			int i = bnum % Neu, ans = W[i][0], t = H[bnum];
			ans = 0;
			for (int j = 1; j <= h; ++j)
			{
				ans += (t & 1) ? W[i][j] : -W[i][j];
				t >>= 1;
			}
			pdct = (ans > 0) ? 1 : 0;
			return pdct;
		}
		case 3:
			//LVQ
			return LVQpredict(bnum);
		case 2:
			//TLAP
			return PHT[bnum][Bhist[bnum]].predict();
		case 1:
			//one bit counter
			return BPmap[bnum];
		case 0:
			//static
			return (tgt > bnum) ? BPmap[bnum] : 1;
		default:
			return 0;
	}
}
void BPFB(int bnum, int taken)
{
	switch (BPno)
	{
		case 4:
		{
			//preceptron
			int i = bnum % Neu;
			if (pdct != taken || (ans < thershold && -ans < thershold))
			{
				W[i][0] += (taken) ? 1 : -1;
//				int t = G;
				int t = H[bnum];
				for (int j = 1; j <= h; ++j)
				{
					W[i][j] += (taken == (t & 1)) ? 1 : -1;
				}
			}
			G = (G << 1) | taken;
			H[bnum] = (H[bnum] << 1) | taken;
			return;
		}
		case 3:
		{
			//LVQ
			HRg = (HRg << 1) | taken;
			HRl[bnum] = (HRl[bnum] << 1) | taken;
			double *Vp = (pdct) ? Vt : Vnt;
			int s = (pdct == taken) ? 1 : -1;
			for (int i = 0; i < lt; ++i)
			{
				Vp[i] += s * aa * (X[i] - Vp[i]);
			}
			aa = aa / (1 + s * aa);
			return;
		}
		case 2:
			//TLAP
			PHT[bnum][Bhist[bnum]].change(taken);
			Bhist[bnum] = ((Bhist[bnum] << 1) | taken) & mask;
			return;
		case 1:
			//onebit counter
		case 0:
			BPmap[bnum] = taken;
			return;
	}
}
#endif
