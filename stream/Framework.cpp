
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "Framework.h"
#include "Task.h"
#include "Bucket.h"

Framework::Framework()
{	
}

Framework::Framework(double pc, double lc)
	: system_power_cap(pc), latency_constraint(lc)
{
}

void Framework::read_profile_config()
{
	std::fstream fin;
//	int tmp;

	fin.open("system.prof", std::ios::in);
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

void Framework::iterate()
{
	// cut result, 0: CPU, 1: FPGA
	bool *ST =  new bool[num_tasks_in_stream];

	task_chain = new Task[num_tasks_in_stream];
	bucket = new Bucket;

	/*
	 * ST should be filled by a max-flow min-cut algorithm,
	 * but we now do it manually when the number of tasks is
	 * small.
	 */
	ST[0] = 1;
	ST[1] = 0;
	ST[2] = 0;

	init_task_chain(ST);
	
	latency_lower_bound = bucket->get_pipeline_latency();
	std::cout << latency_lower_bound << std::endl;
	if (latency_lower_bound > latency_constraint) {
		std::cout << "Latency is too rigid!" << std::endl;
		std::cout << "Exiting...\n";
		exit(1);
	}
	throughput_upper_bound = 1.0 / latency_lower_bound;

	power_lower_bound = 0.0;
	for (int i = 0; i < num_tasks_in_stream; i++)
		power_lower_bound = power_lower_bound + fpga_power[i];
	std::cout << power_lower_bound << std::endl;
	if (power_lower_bound > system_power_cap) {
		std::cout << "Power cap is too rigid!" << std::endl;
		std::cout << "Exiting...\n";
		exit(1);
	}

	delete [] ST;
}

void Framework::init_task_chain(bool *ST)
{
	for (int i = 0; i < num_tasks_in_stream; i++) {
		int id = num_tasks_in_stream - 1 - i;
		
		task_chain[i].set_type(ST[i]);
		task_chain[i].set_sno(num_tasks_in_stream - 1 - i);
		task_chain[i].next = NULL;
		if (ST[i] == 0) {
			// CPU
			task_chain[i].set_time(cpu_time_highest_freq[i]);
			task_chain[i].set_power(cpu_power_highest_freq[i]);
			task_chain[i].set_freq(cpu_freq_scale_space[num_freqs - 1]);
		}
		else {
			// FPGA
			task_chain[i].set_time(fpga_time[i]);
			task_chain[i].set_power(fpga_power[i]);
			task_chain[i].set_freq(0.0);
		}
		bucket->insert(id, &task_chain[i]);
	}
}

Framework::~Framework()
{
	if (task_chain)
		delete [] task_chain;
	if (bucket)
		delete bucket;
}
