
#include <iostream>
#include <cstdlib>
#include <iomanip>
#include "Framework.h"

#include "timer.h"

int main(int argc, char *argv[])
{
	double pc, lc;
	Framework *f;

	double start, stop, elapsed_time;

	if (argc != 3) {
		std::cout << "Usage: ./stream.out power_cap latency_constraint" << std::endl;
		exit(1);
	}
	
	pc = atof(argv[1]);
	lc = atof(argv[2]);
	f = new Framework(pc, lc);
//	f->read_profile_config();
//	f->output_profile_config();
//	f->heuristic();
//	f->heuristic(1);

	start = dtime();
	f->bruteforce();
	stop = dtime();

	elapsed_time = stop - start;
//	f->speak();

	std::cout << std::fixed << std::setprecision(3) << elapsed_time \
		<< "ms" << std::endl;

	delete f;
	
    return 0;
}
