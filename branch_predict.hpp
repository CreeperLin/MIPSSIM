#ifndef _BRANCHPREDICT_
#define _BRANCHPREDICT_

//TLAP
const int N = 4;
int BPno = 2;
string BPdesc[5] = {"static", "onebit", "TLAP", "LVQ"};
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
int Bhist[100000] = {0};
int mask = (1 << N) - 1;
sm PHT[100000][1 << N];

//one bit
int BPmap[100000];

int BP(int bnum, int tgt)
{
	switch (BPno)
	{
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
