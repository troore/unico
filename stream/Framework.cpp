
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>

#include "Framework.h"
#include "Task.h"
#include "Bucket.h"

Framework::Framework()
{
	accepted = true;
}

Framework::Framework(double pc, double lc)
	: system_power_cap(pc), latency_constraint(lc)
{
	accepted = true;
	read_profile_config();
	
	task_chain = new Task[num_tasks_in_stream];
	bucket = new Bucket;
}

void Framework::read_profile_config()
{
	std::fstream fin;
//	int tmp;

	fin.open("lte3.prof", std::ios::in);
//	while (fin >> tmp) {
		//	fin >> tmp;
		//	std::cout << tmp << "\n";
//	}

	// Number of cores on CPU
	fin >> num_cores_on_cpu;

	// Number of frequencies on each CPU core
	fin >> num_freqs;

	cpu_freq_scale_space = new double[num_freqs];

	// frequencies on a CPU core
	for (int i = 0; i < num_freqs; i++)
		fin >> cpu_freq_scale_space[i];

	fin >> num_tasks_in_stream;

	/*
	 * latency and power profiling information on CPU under
	 * highest frequency
	 */ 
	cpu_time_highest_freq = new double[num_tasks_in_stream];
	cpu_power_highest_freq = new double[num_tasks_in_stream];

	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> cpu_time_highest_freq[i];
	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> cpu_power_highest_freq[i];

	/*
	 * latency and power profiling information on FPGA
	 */ 
	fpga_time = new double[num_tasks_in_stream];
	fpga_power = new double[num_tasks_in_stream];
	
	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> fpga_time[i];
	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> fpga_power[i];

	// bandwidth (MBps)
	bandwidth = 800;

	// vector of data size per frame between tasks (x4bytes, int)
	transfer_data_size = new int[num_tasks_in_stream - 1];
	for (int i = 0; i < (num_tasks_in_stream - 1); i++)
		fin >> transfer_data_size[i];

	fin.close();
}

void Framework::output_profile_config()
{
	std::cout << num_cores_on_cpu << std::endl;
	std::cout << num_freqs << std::endl;
	for (int i = 0; i < num_freqs; i++)
		std::cout << cpu_freq_scale_space[i] << " ";
	std::cout << std::endl;
	std::cout << num_tasks_in_stream << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++)
		std::cout << cpu_time_highest_freq[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++)
		std::cout << cpu_power_highest_freq[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++)
		std::cout << fpga_time[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++)
		std::cout << fpga_power[i] << " ";
	std::cout << std::endl;
	for (int i = 0; i < (num_tasks_in_stream - 1); i++)
		std::cout << transfer_data_size[i] << " ";
	std::cout << std::endl;

}

void Framework::clear_profile_config()
{
	if (cpu_freq_scale_space)
		delete [] cpu_freq_scale_space;
	if (cpu_time_highest_freq)
		delete [] cpu_time_highest_freq;
	if (cpu_power_highest_freq)
		delete [] cpu_power_highest_freq;
	if (fpga_time)
		delete [] fpga_time;
	if (fpga_power)
		delete [] fpga_power;
	if (transfer_data_size)
		delete [] transfer_data_size;
}

void Framework::heuristic(int flag)
{
	if (system_power_cap <= 10)
		accepted = false;

	if (latency_constraint >= 60) {
		task_chain[0].set_type(1);
		task_chain[1].set_type(0);
		task_chain[2].set_type(0);
		
		task_chain[0].set_sno(2);
		task_chain[1].set_sno(1);
		task_chain[2].set_sno(0);

		if (system_power_cap >= 20) {
			task_chain[0].set_cpu_freq(0.0);
			task_chain[1].set_cpu_freq(20.2);
			task_chain[2].set_cpu_freq(20.2);
		}
		else {
			task_chain[0].set_cpu_freq(0.0);
			task_chain[1].set_cpu_freq(9.2);
			task_chain[2].set_cpu_freq(9.2);			
		}
	}
	else if (latency_constraint >= 40 && latency_constraint < 60) {
		if (system_power_cap >= 20) {
			task_chain[0].set_type(1);
			task_chain[1].set_type(0);
			task_chain[2].set_type(0);
		
			task_chain[0].set_sno(1);
			task_chain[1].set_sno(0);
			task_chain[2].set_sno(0);
			
			task_chain[0].set_cpu_freq(0.0);
			task_chain[1].set_cpu_freq(20.2);
			task_chain[2].set_cpu_freq(20.2);
		}
		else {
			task_chain[0].set_type(1);
			task_chain[1].set_type(1);
			task_chain[2].set_type(0);
		
			task_chain[0].set_sno(1);
			task_chain[1].set_sno(1);
			task_chain[2].set_sno(0);
			
			task_chain[0].set_cpu_freq(0.0);
			task_chain[1].set_cpu_freq(0.0);
			task_chain[2].set_cpu_freq(9.2);
		}
	}
	else {
		accepted = false;
	}
}

void Framework::heuristic()
{
	// cut result, 0: CPU, 1: FPGA
	bool *ST, *ST_backup;
	
	double cur_latency, cur_throughput, cur_power;

	ST = new bool[num_tasks_in_stream];
	ST_backup = new bool[num_tasks_in_stream];

	/*
	 * ST should be filled by a max-flow min-cut algorithm,
	 * but we now do it manually when the number of tasks is
	 * small.
	 */
	ST[0] = 1;
	ST[1] = 0;
	ST[2] = 0;

	memcpy(ST_backup, ST, num_tasks_in_stream * sizeof(bool));

	init_task_chain(ST);
	// init bucket
	for (int i = 0; i < num_tasks_in_stream; i++) {
		int id = num_tasks_in_stream - 1 - i;
		bucket->insert_task(id, &task_chain[i]);
	}
	
	latency_lower_bound = bucket->get_pipeline_latency();
	std::cout << latency_lower_bound << std::endl;
	if (latency_lower_bound > latency_constraint) {
		accepted = false;
		exit(1);
	}
	throughput_lower_bound = 1.0 / latency_lower_bound;

	power_lower_bound = 0.0;
	for (int i = 0; i < num_tasks_in_stream; i++)
		power_lower_bound = power_lower_bound + fpga_power[i];
	std::cout << power_lower_bound << std::endl;
	if (power_lower_bound > system_power_cap) {
		accepted = false;
		exit(1);
	}

	// main iteration loop
	while (1) {
		// latency part
		cur_latency = bucket->get_pipeline_latency();
		while (cur_latency > latency_constraint) {
			if (!bucket->insert_bubble())
				break;
			cur_latency = bucket->get_pipeline_latency();
		}
		if (cur_latency > latency_constraint) {
			// bubble insertion failed
			accepted = false;
			break;
		}

		/*
		 * Take a look at if current throughput has achieved its lower bound.
		 * If so, recover task chain and do not use pipeline.
		 */
		cur_throughput = bucket->get_pipeline_throughput();
		if (cur_throughput <= throughput_lower_bound) {
			init_task_chain(ST_backup);
			bucket->clear();
			/*
			 * I have to insert all tasks into the 1st pipeline stage to
			 * stop further useless tentative procedure. Because throughput
			 * has achieved its lower bound.
			 */
			for (int i = 0; i < num_tasks_in_stream; i++) {
				bucket->insert_task(0, &task_chain[i]);
			}

			accepted = true;
			
			break;
		}

		// power part
		cur_power = bucket->get_pipeline_power();
		while (cur_power > system_power_cap) {
			if (!dvfs())
				break;
			cur_power = bucket->get_pipeline_power();
		}
		if (cur_power <= system_power_cap) {
			// dvfs successfully
			accepted = true;
			
			break;
		}

		// set default pipeline
		init_task_chain(ST_backup);
		bucket->clear();		

		// try task remapping to balance power between CPU and FPGA
		if (!power_balance(ST)) {
			accepted = false;

			break;
		}

		// remap successfully, go back and continue iteration.
	}

	if (accepted) {
		std::cout << "Accepted\n" << std::endl;
	}
	else {
		std::cout << "Wrong Answer\n" << std::endl;
	}

	delete [] ST;
	delete [] ST_backup;
}

void Framework::init_task_chain(bool *ST)
{
	for (int i = 0; i < num_tasks_in_stream; i++) {
		task_chain[i].set_id(i);
		task_chain[i].set_type(ST[i]);
		task_chain[i].set_sno(num_tasks_in_stream - 1 - i);
		task_chain[i].next = NULL;
		if (ST[i] == 0) {
			// CPU
			task_chain[i].set_time(cpu_time_highest_freq[i]);
			task_chain[i].set_power(cpu_power_highest_freq[i]);
			task_chain[i].set_cpu_freq(cpu_freq_scale_space[num_freqs - 1]);
		}
		else {
			// FPGA
			task_chain[i].set_time(fpga_time[i]);
			task_chain[i].set_power(fpga_power[i]);
			task_chain[i].set_cpu_freq(0.0);
		}
	}
}

void Framework::get_bit_vector(bool *v, int i, int n)
{
	for (int k = 0; k < n; k++) {
		v[n - k - 1] = (i & 1);
		i >>= 1;
	}
}

void Framework::DFS_freq_space(std::vector<Task *>tv, int k, int n)
{
	if (k == n)
		return;
	for (int i = 0; i < num_freqs; i++) {
		tv[k]->set_cpu_freq_cursor(i);
		DFS_freq_space(tv, k + 1, n);
	}
}

void Framework::bruteforce()
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
					bucket->insert_task(sno, &task_chain[k]);
				}
				start_id = stop_id + 1;
				sno++;
			}
		}
		bucket->set_ns(sno);

		/*
		 * The second hierarchy search: for each pipeline stage,
		 * enumerate processor type for each task in this stage.
		 */
		for (int j = 0; j < bound1; j++) {
			get_bit_vector(v1, j, num_tasks_in_stream);
			for (int k = 0; k < num_tasks_in_stream; k++) {
				task_chain[k].set_type(v1[k]);
			}

			/*
			 * The third hierarchy search: for each task mapped onto CPU,
			 * enumerate available frequencies. As the frequency search space
			 * is larger than 2, we use DFS.
			 */
			std::vector<Task *> cpu_task_vector;
			int cpu_task_num = 0;

			for (int k = 0; k < num_tasks_in_stream; k++) {
				if (0 == v1[i])
					cpu_task_vector.push_back(&task_chain[i]);
			}
			cpu_task_num = cpu_task_vector.size();

			DFS_freq_space(cpu_task_vector, 0, cpu_task_num);
		}

		/*
		  * Clear footprint for this division.
		  */
		bucket->clear();
	}

	delete [] v0;
	delete [] v1;
}

bool Framework::dvfs()
{
	double min_time = -1.0;
	int min_tid;
	int min_cursor;
	
	for (int i = 0; i < num_tasks_in_stream; i++) {
		min_cursor = task_chain[i].get_cpu_freq_cursor();
		if (min_cursor > 0) {
			// there is frequency scaling space
			double tmp = task_chain[i].get_time();
			if ((min_time < 0) || (tmp < min_time)) {
				min_time = tmp;
				min_tid = task_chain[i].get_id();
			}
		}
	}

	if (min_time > 0) {
		/*
		  * There is a task on which we could do frequency scaling.
		  * And this task has the smallest latency among those which
		  * could stand frequency scaling.
		  */
		double time;
		double cpu_freq;
		double diff;
		int sno;

		sno = task_chain[min_tid].get_sno();
		time = bucket->get_stage_len(sno) - task_chain[min_tid].get_time() + task_chain[min_tid].get_time_at_cpu_freq(cpu_freq_scale_space[min_cursor - 1]);

		diff = time - bucket->get_max_stage_len();
		if (diff < 0 || diff < 1e-3) {
			// diff <= 0
			double power = task_chain[min_tid].get_power_at_cpu_freq(cpu_freq_scale_space[min_cursor - 1]);
			
			task_chain[min_tid].set_cpu_freq_cursor(min_cursor - 1);
			task_chain[min_tid].set_time(time);
			task_chain[min_tid].set_power(power);

			return true;
		}
	}

	return false;
}

bool Framework::power_balance(bool *ST)
{
	double stream_latency, latency_increment;
	double stream_power, power_decrement;
	double stream_energy, energy_decrement;
	int id;

	stream_latency = 0.0;
	stream_power = 0.0;
	stream_energy = 0.0;
	for (int i = 0; i < num_tasks_in_stream; i++) {
		if (ST[i] == 1) {
			stream_latency += cpu_time_highest_freq[i];
			stream_energy += cpu_time_highest_freq[i] * cpu_power_highest_freq[i];
		}
		else {
			stream_latency += fpga_time[i];
			stream_energy += fpga_time[i] * fpga_time[i];
		}
	}
	stream_power = (stream_energy / stream_latency);

	double new_latency, new_energy, new_power;
	double lop = 1e7;
	id = -1;
	for (int i = 0; i < num_tasks_in_stream; i++) {
		if (ST[i] == 0) {
			new_latency = stream_latency - cpu_time_highest_freq[i] + fpga_time[i];
			new_energy = stream_energy - cpu_time_highest_freq[i] * cpu_power_highest_freq[i] + fpga_time[i] * fpga_power[i];
			new_power = new_energy / new_latency;
			
			latency_increment = new_latency - stream_latency;
			power_decrement = stream_power - new_power;

			if (power_decrement < 1e-6) {
				// take 1.0 as power consumption caused by communication
				// between CPU and FPGA
				power_decrement = 1.0;
			}
			if ((latency_increment / power_decrement) < lop) {
				id = i;
			}
		}
	}

	if (id >= 0) {
		ST[id] = 0;

		return true;
	}
	else {
		// there is no power benefit to do balancing
		return false;
	}
}

void Framework::speak()
{
	if (!accepted) {
		std::cout << "Constraints are too rigid.\n";
		
		return;
	}
	std::cout << "#tid\t#sid\t#pid\tfreq" << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++) {
		std::cout << i << "\t";
		std::cout << task_chain[i].get_sno() << "\t";
		std::cout << task_chain[i].get_type() << "\t";
		std::cout << task_chain[i].get_cpu_freq() << "\t";
		std::cout << "\n";
	}
}	

Framework::~Framework()
{
	if (task_chain)
		delete [] task_chain;
	if (bucket)
		delete bucket;
}
