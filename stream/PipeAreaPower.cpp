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

PipeAreaPower::PipeAreaPower(double pc, double ac, int nt, int nc, bool flag)
	: PipeModel(nt, nc, flag), system_power_cap(pc), area_constraint(ac)
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
		Speak();
		
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
		double thr = pipe->get_pipeline_thr();

		if (pipe_optthr < thr)
			pipe_optthr = thr;

		return;
	}

	int proc_type = tv[k].get_proc_type();
	
	if (0 == proc_type) {
		// CPU
		int num_freqs = tv[k].get_num_freqs();

		for (int i = 0; i < num_freqs; i++) {
			tv[k].set_cursor(i);
			DFS(tv, k + 1, n);
		}
	}
	else {
		// FPGA
		int num_areas = tv[k].get_num_areas();

		for (int i = 0; i < num_areas; i++) {
			tv[k].set_cursor(i);
			DFS(tv, k + 1, n);
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

	/*
	 * The first hierarchy search
	 */
	v0[0] = v0[num_tasks_in_stream] = 1;
	for (int i = 0; i < bound0; i++) {
		get_bit_vector(v0 + 1, i, num_tasks_in_stream - 1);
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
		for (int j = 0; j < bound1; j++) {
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
			DFS(task_chain, 0, num_tasks_in_stream);
			
			//	pipe_thr = 0.0;
			//	DFSOptThr(task_chain, 0, num_tasks_in_stream);
		}

		/*
		 * Clear footprint for this division.
		 */
		pipe->clear();
	}

	delete [] v0;
	delete [] v1;
}

/*
 * Dynamic programming
 */
void PipeAreaPower::DP()
{
}

void PipeAreaPower::Speak()
{
	/*
	std::cout << "Thr:\t" << pipe->get_pipeline_thr() << std::endl;
	std::cout << "Area:\t" << pipe->get_pipeline_area() << std::endl;
	std::cout << "Power:\t" << pipe->get_pipeline_power() << std::endl;

	PipeModel::Speak();
	*/
//	std::cout << pipe->get_pipeline_area() << "\t";
	std::cout << pipe->get_pipeline_power() << "\t";
	std::cout << pipe->get_pipeline_area() << "\t";
	std::cout << pipe->get_pipeline_thr() << "\n";
	
}
