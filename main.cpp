#include <bits/stdc++.h>
#include "main.h"
#include "assembler.cpp"
#include "mipsim.hpp"
int main(int argc,char* argv[])
{
	freopen("my.out", "w", stdout);
	init();
	string aspath, inpath, outpath;
	aspath = argv[1];
	inpath = aspath + ".in";
	outpath = aspath + ".ans";
	aspath += ".s";
	fin.open(inpath);
	fas.open(aspath);
	fout.open(outpath);
	assert(fas.is_open() && fin.is_open() && fout.is_open());
	compile();
	cout << "compiled!" << endl;

	run();

	fin.close();
	fout.close();
	return 0;
}

