#include <bits/stdc++.h>
#include "main.h"
#include "assembler.cpp"
#include "mipsim.hpp"
int main(int argc, char* argv[])
{
	freopen("myerr", "w", stderr);
	init();

//	fas.open(argv[1]);
//	assert(fas.is_open());
#if MANUAL
	string asp = "1";
	fas.open(asp + ".s");
	fstat.open(asp + ".stat", ios::app);
	fin.open(asp + ".in");
	fout.open(asp + ".out");
	assert(fas.is_open() && fout.is_open());
#else
	if (argc < 2)
	{
		cout << "no input file, exiting" << endl;
		return 0;
	}
	fas.open(argv[1]);
	string s = argv[1];
	fstat.open(s + ".stat", ios::app);
	assert(fas.is_open());
#endif
	int m = compile();
	cerr << "compiled!" << endl;
	run(m);
#if MANUAL
	fin.close();
	fout.close();
#endif
	return 0;
}

