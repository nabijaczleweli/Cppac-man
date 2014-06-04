#include <cstdlib>
#include <cstring>
#include <fstream>
#include "../ASCII charcodes.hpp"

using namespace std;

int main(int, char * argv[]) {
	string output_filename = "file0.lvl";
	int ghost_amount = 1;

	for(unsigned int idx = 1; argv[idx]; ++idx) {
		const char * const arg = argv[idx];
		const unsigned int strlen_arg = strlen(arg);
		if(*arg == '-' && arg[1]) {
			if(strlen_arg > 9 && memcmp(arg, "--ghosts=", 9))
				ghost_amount = atoi(arg);
		} else
			output_filename = arg;
	}

	if(ghost_amount <= 0)
		ghost_amount = 1;

	printf("Making level-file of name \"%s\" with %d ghosts.\nNote: you MUST edit the file and put the proper data into it.", output_filename.c_str(), ghost_amount);

	ofstream ofile(output_filename);
	ofile << CHR_SOH << CHR_SI << "<lvl he>,<lvl wi>" << CHR_SO << CHR_EM << CHR_SI << ghost_amount;
	for(int i = 0; i < ghost_amount; ++i)
		ofile << CHR_ETB << "<ghost pos x>,<ghost pos y>,<attrs>";
	ofile << CHR_SO << CHR_EM << CHR_STX << '_';
}