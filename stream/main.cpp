
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include "PipeModel.h"
//#include "PipeLatPower.h"
#include "PipeAreaPower.h"

#include "timer.h"

int main(int argc, char *argv[])
{
	double pc;
	double lc, ac;

	PipeAreaPower *f;

	char fname[50];

	double start, stop, elapsed_time;

	if (argc != 4) {
		std::cout << "Usage: ./stream.out filename power_cap area_constraint" << std::endl;
		exit(1);
	}
	
	strcpy(fname, argv[1]);
	pc = atof(argv[2]);
	lc = atof(argv[3]);

//	f = new PipeLatPower(pc, lc);
	f = new PipeAreaPower(pc, ac);

	f->ReadProfileConfig(fname);
	f->PrintProfileConfig();

	start = dtime();

//	f->BB();
//	f->DP();

	stop = dtime();

	elapsed_time = stop - start;
//	f->speak();

	std::cout << std::fixed << std::setprecision(3) << elapsed_time \
		<< "ms" << std::endl;

	delete f;
	
    return 0;
}
