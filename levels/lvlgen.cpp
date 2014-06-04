#include <cstdlib>
#include <fstream>
#include "../ASCII charcodes.hpp"

using namespace std;

int main(int argc, char * argv[]) {
	ofstream ofile(argv[argc - 1]);
	unsigned int ghost_amount = atoi(argv[argc - 2]);
	ofile << CHR_SOH << CHR_SI << "<lvl he>,<lvl wi>" << CHR_SO << CHR_EM << CHR_SI << ghost_amount;
	for(unsigned i = 0; i < ghost_amount; ++i)
		ofile << CHR_ETB << "<ghost pos x>,<ghost pos y>,<attrs>";
	ofile << CHR_SO << CHR_EM << CHR_STX;
}