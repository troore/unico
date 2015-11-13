
#include <iostream>
#include <cstdlib>
#include "Framework.h"

int main(int argc, char *argv[])
{
	double pc, lc;
	Framework *f;

	if (argc != 3) {
		std::cout << "Usage: ./stream.out power_cap latency_constraint" << std::endl;
		exit(1);
	}
	
	pc = atof(argv[1]);
	lc = atof(argv[2]);
	f = new Framework(pc, lc);
	f->read_profile_config();
//	f->output_profile_config();
//	f->iterate();
	f->iterate(1);
	f->speak();

	delete f;
	
    return 0;
}
