
#include <iostream>
#include <fstream>
#include <cmath>
#include "Framework.h"

Framework::Framework()
{	
	latency_lower_bound = 0;
	throughput_upper_bound = 0;
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

	// Bandwidth
	fin >> bdw_cpu_fpga >> bdw_fpga_cpu;

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
	cpu_latency_under_highest_freq = new double[num_tasks_in_stream];
	cpu_power_under_highest_freq = new double[num_tasks_in_stream];

	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> cpu_latency_under_highest_freq[i];
	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> cpu_power_under_highest_freq[i];

	/*
	 * latency and power profiling information on FPGA under
	 * highest resource usage
	 */ 
	fpga_latency_under_highest_resusg = new double[num_tasks_in_stream];
	fpga_power_under_highest_resusg = new double[num_tasks_in_stream];
	
	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> fpga_latency_under_highest_resusg[i];
	for (int i = 0; i < num_tasks_in_stream; i++)
		fin >> fpga_power_under_highest_resusg[i];

	transfer_data_size = new int[num_tasks_in_stream - 1];
	for (int i = 0; i < (num_tasks_in_stream - 1); i++)
		fin >> fpga_power_under_highest_resusg[i];

	if (fin.eof())
		std::cout << "[EoF reached]\n";
	else
		std::cout << "[error reading]\n";

	fin.close();
}

int Framework::init_predict_model()
{
	// profiling results of the first task
	double p = cpu_power_under_highest_freq[0];
	double l = cpu_latency_under_highest_freq[0];
	double f = cpu_freq_scale_space[num_freqs - 1];
	
	lambda = p / pow(f, 3.0);
	mu = l * f;

	for (int i = 0; i < num_tasks_in_stream; i++) {
		int j = num_freqs - 1;
		/*
		 * tentatively try a lower frequency
		 */
		while (p >= cpu_power_cap && j > 0) {
			j = j - 1;
			f = cpu_freq_scale_space[j];
			p = lambda * pow(f, 3.0);
		}
		if (j <= 0)
			return 1;
		else {
			cpu_power_under_highest_freq[i] = p;
			cpu_latency_under_highest_freq[i] = mu * f;
		}
	}
	for (int i = 0; i < num_tasks_in_stream; i++) {
		l = cpu_latency_under_highest_freq[i];
		if (l >= latency_constraint)
			return 2;
	}

	return 0;
}

void Framework::set_default_bounds()
{
	double *aa, *bb, *ws, *wt;

	int n = num_tasks_in_stream;
	int m = n - 1;

	aa = new double[m];
	bb = new double[m];
	ws = new double[m];
	wt = new double[m];

	for (int i = 0; i < m; i++) {
		aa[i] = i;
		bb[i] = i + 1;
		ws[i] = transfer_data_size[i] / bdw_cpu_fpga;
		wt[i] = transfer_data_size[i] / bdw_fpga_cpu;
	}

	dinic = new Dinic(n, m);
	
	dinic.cst_graph(cpu_latency_under_highest_freq, fpga_latency_under_highest_freq,
					aa, bb, ws, wt, n, m);
	latency_lower_bound = dinic.max_flow(S, T);
	throughput_upper_bound = (1.0 / latench_lower_bound);

	delete[] aa;
	delete[] bb;
	delete[] ws;
	delete[] wt;
}

void Framework::iterate()
{
	
}
Framework::~Framework()
{
	delete[] cpu_freq_scale_space;
	delete[] cpu_latency_under_highest_freq;
	delete[] cpu_power_under_highest_freq;
	delete[] fpga_latency_under_highest_resusg;
	delete[] fpga_power_under_highest_resusg;
	delete[] transfer_data_size;

	if (dinic)
		delete dinic;
}
