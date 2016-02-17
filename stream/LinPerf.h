#ifndef __LINPERF_H_
#define __LINPERF_H_

#include "PipeModel.h"

class LinPerf : public PipeModel {
private:
	// output
	double lin_lat_opt;
	double lin_eng_opt;
public:
	LinPerf();
	~LinPerf();

	void BB();
	void ReportOpt();
};

#endif
