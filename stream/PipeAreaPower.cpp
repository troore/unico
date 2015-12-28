#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>

#include "PipeAreaPower.h"

PipeAreaPower::PipeAreaPower(double pc, double lc)
	: system_power_cap(pc), area_constraint(lc)
{
}

PipeAreaPower::~PipeAreaPower()
{
}

void PipeAreaPower::DFS(std::vector<Task> &tv, int k, int n)
{
	if (k == n)
		return;

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
	bool *v0 = new bool[num_tasks_in_stream - 1];
	bool *v1 = new bool[num_tasks_in_stream];
	
	int bound0 = (int)pow(2.0, (num_tasks_in_stream - 1));
	int bound1 = (int)pow(2.0, num_tasks_in_stream);

	/*
	 * The first hierarchy search
	 */
	for (int i = 0; i < bound0 - 1; i++) {
		get_bit_vector(v0, i, num_tasks_in_stream - 1);
		/*
		 * Divide the stream into pipeline stages and 
		 * insert the tasks of each pipeline stage into
		 * its individual Bucket.
		 */
		int start_id = 0, stop_id = 0;
		int sno = 0;
		
		for (int j = 1; j <= (num_tasks_in_stream - 1); j++) {
			if (v0[j] == 1) { // a bound is encountered
				stop_id = j - 1;
				// insert tasks
				for (int k = start_id; k <= stop_id; k++) {
					pipe->insert_task(sno, &task_chain[k]);
				}
				start_id = stop_id + 1;
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
			}

			/*
			 * The third hierarchy search: for each task mapped onto CPU,
			 * enumerate available frequencies. As the frequency and area
			 * search space is larger than 2, we use DFS.
			 */
			DFS(task_chain, 0, num_tasks_in_stream);
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
{}
