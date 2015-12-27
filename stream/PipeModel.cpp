
#include <iostream>
#include <fstream>

#include "PipeModel.h"

PipeModel::PipeModel()
{
}

void PipeModel::ReadProfileConfig(char *fname)
{
	std::fstream fin;
//	int tmp;

//	fin.open("jpeg6.prof", std::ios::in);
	fin.open(fname, std::ios::in);
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

void PipeModel::OutputProfileConfig()
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

void PipeModel::ClearProfileConfig()
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

void PipeModel::get_bit_vector(bool *v, int i, int n)
{
	for (int k = 0; k < n; k++) {
		v[n - k - 1] = (i & 1);
		i >>= 1;
	}
}

PipeModel::~PipeModel()
{
}
