#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <bitset>

#include "PipeAreaPower.h"

PipeAreaPower::PipeAreaPower()
{
}

PipeAreaPower::PipeAreaPower(double pc, double ac, int nt, int nc)
	: PipeModel(nt, nc), system_power_cap(pc), area_constraint(ac)
{
}

PipeAreaPower::~PipeAreaPower()
{
}

/*
 * Depth first search to do design space exploration.
 */
void PipeAreaPower::DFS(std::vector<Task> &tv, int k, int n)
{
	if (k == n) {
		// Have assigned all tasks a frequency or an area
		Report();
		
		return;
	}

	int proc_type = tv[k].get_proc_type();
	
	if (0 == proc_type) {
		// CPU
		int num_freqs = tv[k].get_num_freqs();

		for (int i = 0; i < num_freqs; i++) {
			tv[k].set_cursor(i);
			pipe->update_stage(pipe->get_sno_tid(k));
			DFS(tv, k + 1, n);
		}
	}
	else {
		// FPGA
		int num_areas = tv[k].get_num_areas();

		for (int i = 0; i < num_areas; i++) {
			tv[k].set_cursor(i);
			pipe->update_stage(pipe->get_sno_tid(k));
			DFS(tv, k + 1, n);
		}
	}
}

/*
 * Depth first search to get optimized throughput
 * with pruning directed by power and area constraints.
 */
void PipeAreaPower::DFSOptThr(std::vector<Task> &tv, int k, int n)
{
	if (k == n) {
		int area = pipe->get_pipeline_area();
		double power = pipe->get_pipeline_power();
		double thr = pipe->get_pipeline_thr();

		//	printf("%.3f %d\n", power, area);

		if (power <= system_power_cap && area <= area_constraint) {
			if (pipe_thr_opt < thr) {
				pipe_thr_opt = thr;
				pipe_area_opt = area;
				pipe_power_opt = power;
				// record
				for (int i = 0; i < num_tasks_in_stream; i++) {
					lwtask_chain[i] = task_chain[i];
				}
			}
		}

		return;
	}

	int proc_type = tv[k].get_proc_type();
	
	if (0 == proc_type) {
		// CPU
		int num_freqs = tv[k].get_num_freqs();

		for (int i = 0; i < num_freqs; i++) {
			tv[k].set_cursor(i);
			pipe->update_stage(pipe->get_sno_tid(k));
			DFSOptThr(tv, k + 1, n);
		}
	}
	else {
		// FPGA
		int num_areas = tv[k].get_num_areas();

		for (int i = 0; i < num_areas; i++) {
			tv[k].set_cursor(i);
			pipe->update_stage(pipe->get_sno_tid(k));
			DFSOptThr(tv, k + 1, n);
		}
	}
}

/*
 * Branch and bound
 */
void PipeAreaPower::BB()
{
	bool *v0 = new bool[num_tasks_in_stream + 1];
	bool *v1 = new bool[num_tasks_in_stream];
	
	int bound0 = (int)pow(2.0, (num_tasks_in_stream - 1));
	int bound1 = (int)pow(2.0, num_tasks_in_stream);

	pipe_thr_opt = -1.0;
	/*
	 * The first hierarchy search
	 */
	v0[0] = v0[num_tasks_in_stream] = 1;
	for (int i = 0; i < bound0; i++) {
		get_bit_vector(v0 + 1, i, num_tasks_in_stream - 1);

		int nc = 1;
		
		for (int j = 1; j < num_tasks_in_stream; j++) {
			if (v0[j])
				nc++;
		}
		if (nc > num_cores_on_cpu)
			continue;
		
		/*
		 * Divide the stream into pipeline stages and 
		 * insert the tasks of each pipeline stage into
		 * its individual Bucket.
		 */
		int start_id = 0, stop_id = 0;
		int sno = 0;
		
		while (++stop_id <= num_tasks_in_stream) {
			if (v0[stop_id] == 1) { // a bound is encountered
				// insert tasks
				for (int k = start_id; k < stop_id; k++) {
					pipe->insert_task(sno, &task_chain[k]);
					task_chain[k].set_sno(sno);
				}
				start_id = stop_id;
				sno++;
			}
		}
		pipe->set_ns(sno);

		/*
		 * The second hierarchy search: for each pipeline stage,
		 * enumerate processor type for each task in this stage.
		 */
		for (int j = 0 /*bound1 - 1*/; j < bound1 /*1*/; j++) {
			get_bit_vector(v1, j, num_tasks_in_stream);
			for (int k = 0; k < num_tasks_in_stream; k++) {
				task_chain[k].set_proc_type(v1[k]);
				pipe->update_stage(pipe->get_sno_tid(k));
			}

			/*
			 * The third hierarchy search: for each task mapped onto CPU,
			 * enumerate available frequencies. As the frequency and area
			 * search space is larger than 2, we use DFS.
			 */
			//	DFS(task_chain, 0, num_tasks_in_stream);
			
			DFSOptThr(task_chain, 0, num_tasks_in_stream);
		}

		/*
		 * Clear footprint for this division.
		 */
		pipe->clear();
	}

	ReportOpt();

	delete [] v0;
	delete [] v1;
}

/*
 * Dynamic programming
 */
void PipeAreaPower::DP()
{
	/*
	 * Memoriozation initialization
	 */

	/*
	 * Process
	 */
	int P = (int)(system_power_cap * 100);
	int A = area_constraint;
	int m = num_cores_on_cpu;
	int n = num_tasks_in_stream;
	
	for (;;) {
	}
}

void PipeAreaPower::Report()
{
	std::cout << "Thr:\t" << pipe->get_pipeline_thr() << std::endl;
	std::cout << "Area:\t" << pipe->get_pipeline_area() << std::endl;
	std::cout << "Power:\t" << pipe->get_pipeline_power() << std::endl;

	PipeModel::Report();
}

void PipeAreaPower::ReportOpt()
{
	if (pipe_thr_opt < 0){
		std::cout << "Cannot satisfy constraints." << std::endl;
	}
	else {
		std::cout << "ThrOpt:\t" << pipe_thr_opt << std::endl;
		std::cout << "AreaOpt:\t" << pipe_area_opt << std::endl;
		std::cout << "PowerOpt:\t" << pipe_power_opt << std::endl;

		PipeModel::ReportOpt();
	}
}
