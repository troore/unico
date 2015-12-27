#ifndef __PIPEMODEL_H_
#define __PIPEMODEL_H_

#include <vector>

#include "Task.h"
#include "Bucket.h"

#define MAX_TASK_NUM 10

class PipeModel {
protected:
	// number of cores
	int num_cores_on_cpu;
	// number of frequencies
	int num_freqs;
	double *cpu_freq_scale_space;
	// number of tasks in a stream
	int num_tasks_in_stream;
	// vector of task latencies on CPU with highest frequency
	double *cpu_time_highest_freq;
	// vector of task powers on CPU with highest frequency
	double *cpu_power_highest_freq;
	// vector of tasks latencies on FPGA
	double *fpga_time;
	// vector of task powers on FPGA
	double *fpga_power;
	// vector of task areas on FPGA
	int *fpga_area;

	double bandwidth;
	// in bytes
	int *transfer_data_size;
	
public:
	PipeModel();
	~PipeModel();
	void ReadProfileConfig(char *fname);
	void OutputProfileConfig();
	void ClearProfileConfig();
	void get_bit_vector(bool *v, int i, int n);
};

#endif
