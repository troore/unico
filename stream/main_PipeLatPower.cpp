
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include "PipeModel.h"
#include "PipeLatPower.h"

#include "timer.h"

int main(int argc, char *argv[])
{
	double pc, lc;
//	Framework *f;
	PipeLatPower *f;

	char fname[50];

	double start, stop, elapsed_time;

	if (argc != 4) {
		std::cout << "Usage: ./PipeLatPower.out filename power_cap latency_constraint" << std::endl;
		exit(1);
	}
	
	strcpy(fname, argv[1]);
	pc = atof(argv[2]);
	lc = atof(argv[3]);
//	f = new Framework(pc, lc);
	f = new PipeLatPower(pc, lc);

	f->ReadProfileConfig(fname);

	start = dtime();

//	f->output_profile_config();
//	for (int h = 0; h < 1000; h++)
//	f->heuristic();

	f->BranchBound();

	stop = dtime();

	elapsed_time = stop - start;
//	f->speak();

	std::cout << std::fixed << std::setprecision(3) << elapsed_time \
		<< "ms" << std::endl;

	delete f;
	
    return 0;
}
