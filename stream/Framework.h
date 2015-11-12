#ifndef __FRAMEWORK_H_
#define __FRAMEWORK_H_

#include "Task.h"
#include "Bucket.h"

#define MAX_TASK_NUM 10

class Framework {
private:
	double system_power_cap;
	double latency_constraint;
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

	double bandwidth;
	// in bytes
	int *transfer_data_size;
	
	double latency_lower_bound;
	double throughput_lower_bound;
	double power_lower_bound;
	
	// task chain
	Task *task_chain;
	// bucket to simulate the pipeline
//	Bucket<Task> *bucket;
	Bucket *bucket;
	
	// output
	double time;
	double power;
public:
	Framework();
	Framework(double pc, double lc);
	~Framework();
	void read_profile_config();
	void output_profile_config();
	void clear_profile_config();
	void init_task_chain(bool *ST);
	void iterate();
	bool dvfs();
	bool power_balance(bool *ST);
};

#endif
