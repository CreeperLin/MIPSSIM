#include <bits/stdc++.h>
#include "main.h"
#include "assembler.cpp"
#include "mipsim.hpp"
int main(int argc,char* argv[])
{
	freopen("myerr", "w", stderr);
	init();
	fas.open(argv[1]);
	assert(fas.is_open());
	compile();
	cerr << "compiled!" << endl;

	run();
	return 0;
}

