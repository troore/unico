#ifndef __FRAMEWORK_H_
#define __FRAMEWORK_H_

#include "PriorityQ.h"
#include "Dinic.h"
#include "DisjointSet.h"
#include "Task.h"

class Framework {
	double system_power_cap;
	double cpu_power_cap;
	double fpga_power_cap;
	double latency_constraint;
	int num_cores_on_cpu;
	int num_tasks_in_stream;
	double bdw_cpu_fpga, bdw_fpga_cpu;

	Task *task_chain;
	// in bytes
	int *transfer_data_size;

	/*
	 * performance and power prediction models
	 * power = lambda*freq^3
	 * latency = mu/freq
	 */
	double lambda;
	double mu;

	int num_freqs;
	double *cpu_freq_scale_space;
	// in millisecond
	double *cpu_latency_under_highest_freq;
	double *fpga_latency_under_highest_resusg;
	// in watt
	double *cpu_power_under_highest_freq;
	double *fpga_power_under_highest_resusg;
	
	double latency_lower_bound;
	double throughput_upper_bound;

	// cut result
	int *S, *T;
	/*
	 * vector stores map result:
	 * 0: CPU
	 * 1: FPGA
	 */
	int *v_map_result;
	
	PriorityQ *priq_bak, *priq;
	Dinic *dinic;
	DisjointSet *disjset_bak, *disjset;
public:
	Framework();
	~Framework();
	void read_profile_config();
	int init_predict_model();
	void init_task_chain();
	void set_default_bounds();
	void iterate();
	bool DVFS_sync();
	bool DVFS_async();
};

#endif
