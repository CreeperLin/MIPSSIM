#include <bits/stdc++.h>
#include "main.h"
#include "assembler.cpp"
int main()
{
	freopen("my.out","w",stdout);
	init();
	string aspath, inpath, outpath;
	aspath="2.s";
//	fin.open(inpath);
//	fout.open(outpath);
	fas.open(aspath);
	assert(fas.is_open());
	compile();
	cout << "compiled!" << endl;
//	run();
//	fin.close();
//	fout.close();
	return 0;
}

