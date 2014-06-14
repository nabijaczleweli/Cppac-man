// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include "../ASCII charcodes.hpp"

using namespace std;

int main(int, char * argv[]) {
	string output_filename = "file0.lvl";
	int ghost_amount = 1;
	int wall_amount = 10;

	for(unsigned int idx = 1; argv[idx]; ++idx) {
		const char * const arg = argv[idx];
		if(*arg == '-' && arg[1]) {
			if(!memcmp(arg, "--ghosts=", 9))
				ghost_amount = atoi(arg + 9);
			else if(!memcmp(arg, "--walls=", 8))
				wall_amount = atoi(arg + 8);
		} else
			output_filename = arg;
	}

	if(ghost_amount <= 0)
		ghost_amount = 1;
	if(wall_amount <= 0)
		wall_amount = 10;

	printf("Making level-file of name \"%s\" with %d ghost%s and %d wall%s.\nNote: you MUST edit the file and put the proper data into it.", output_filename.c_str(), ghost_amount,
	       (ghost_amount - 1) ? "s" : "", wall_amount, (wall_amount - 1) ? "s" : "");

	ofstream ofile(output_filename);
	ofile << CHR_SOH << CHR_SI << "<lvl he>,<lvl wi>" << CHR_SO << CHR_EM << CHR_SI << ghost_amount;
	for(int i = 0; i < ghost_amount; ++i)
		ofile << CHR_ETB << "<ghost pos x>,<ghost pos y>,<attrs>,<ghost logic library path>."
#ifdef _WIN32
	"dll"
#elif defined(__APPLE__)
	"dylib"
#else
	"so"
#endif
	;
	ofile << CHR_SO << CHR_EM << CHR_SI << wall_amount;
	for(int i = 0; i < wall_amount; ++i)
		ofile << CHR_ETB << "<wall pos x>,<wall pos y>";
	ofile << CHR_SO << CHR_EM << CHR_SI << "<pac pos x>,<pac pos y>" << CHR_SO << CHR_STX << '_';
}
