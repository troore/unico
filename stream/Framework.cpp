
#include <iostream>
#include <fstream>
#include <stack>
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

void Framework::init_task_chain()
{
	task_chain = new Task[num_tasks_in_stream];

	/*
	 * Initiate the task chain.
	 */
	for (int i = 0; i < num_tasks_in_stream; i++) {
		double l, p;
		
		task_chain[i]->set_id(i, v_map_result[i]);
		if (0 == v_map_result[i]) {
			// CPU
			l = cpu_latency_under_highest_freq[i];
			p = cpu_power_under_highest_freq[i];
			task_chain[i]->set_latency(l);
			task_chain[i]->set_power(p);
		}
		else {
			// FPGA
			l = fpga_latency_under_highest_freq[i];
			task_chain[i]->set_latency(l);
			/*
			  * Use lowest CPU frequency when there is a FPGA task.
			  */
			p = fpga_power_under_highest_freq[i] +
				lambda * pow(cpu_freq_scale_space[0], 3.0);
			task_chain[i]->set_power(p);
		}
		task_chain[i]->set_lop();
	}

	/*
	 * Initiate disjoint set.
	 */
	disjset = new DisjointSet(num_tasks_in_stream);
	/*
	for (int i = 0; i < num_tasks_in_stream; i++) {
		disjset->make_set(i);
	}
	*/
	disjset->build_disjset();

	/*
	 * Initiate priority queue.
	 */
	priq->Build_Min_Heap(task_chain);
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
	
	dinic->cst_graph(cpu_latency_under_highest_freq, fpga_latency_under_highest_freq,
					aa, bb, ws, wt, n, m);
	latency_lower_bound = dinic->max_flow(S, T);
	throughput_upper_bound = (1.0 / latench_lower_bound);

	delete[] aa;
	delete[] bb;
	delete[] ws;
	delete[] wt;
}

void Framework::iterate()
{
	bool accepted = false;
	double stg_len, next_stg_len, throughput;
	double system_power;
	int i;
	i = num_tasks_in_stream;

	while (!accepted) {
		stg_len = get_stage_length();
		next_stg_len = get_next_stage_length();
		throughput = 1.0 / stg_len;
		if (stg_len >= next_stg_len || i <= 0) {
			recover_stream();
			insert_bubble();
		}
		if (throughput > throughput_upper_bound) {
			throughput = throughput_upper_bound;
			accepted = true;
		}
		else {
			system_power = get_system_power_consumption();
			while (system_power > system_power_cap) {
				DVFS();
				stg_len = get_stage_length();
				if (stg_len >= next_stg_len || i <= 0)
					break;
			}
			if (system_power <= system_power_cap) {
				accepted = true;
			}
			else {
				dinic->cst_graph(task_chain);
				dinic->max_flow(S, T, &cnts, &cntt);
			}
		}
	}
}

double Framework::get_stage_length()
{
	return disjset->max_set_size();
}

double Framework::get_system_power_consumption()
{
	return disjset->system_power_consumption();
}

void Framework::recover_stream()
{
	priq = priq_bak;
	disjset = disjset_bak;
}

double Framework::insert_bubble()
{
	// bubble insertion
	int core_id, neb_core_id;
	double next_stg_len_bound;

	core_id = disjset->min_set_size_id();
	neb_core_id = disjset->neb_set_id(core_id);

	disjset->union_set(core_id, neb_core_id);

	disjset_bak = disjset;
	core_id = disjset_bak->min_latency_core_id();
	next_stg_len_bound = disjset_bak->min_set_latency();
	neb_core_id = disjset_bak->neb_core_id(core_id);

	disjset_bak->union_set(core_id, neb_core_id);
	
	// reserve mapping and scheduling results
	// before DVFS and remapping and rescheduling
	disjset_bak = disjset;

	return next_stg_len_bound;
}

bool Framework::DVFS_sync()
{}

bool Framework::DVFS_async(double stg_len_limit)
{
	std::stack<Task *> buffer;
	Task *pt;
	int tid, fid, fid_new;
	double l_task, l_set, l_set_new;
	bool flag;

	flag = false;
	while (pt = priq->Heap_Min_Key()) {
		id = pt->get_id();
		fid = pt->get_freq_id();
		if (fid + 1 < num_freqs) {
			// if there is frequency scaling space, try to do it
			fid_new = fid + 1;
			l_set = disjset->get_min_set_latency(id);
			l_task = pt->get_latency();
			l_tmp = l - l_task + lambda * pow(fid_new, 3.0);
			if (l_tmp > stg_len_limit) {
				buffer.push(priq->min_heap_extract_top());
			}
			else {
				// do DVFS
				//	lop_task_new = l_task_new / p_task_new;
				pt->set_latency(l_task_new);
				pt->set_power(p_task_new);
				pt->set_lop();
				priq->min_heap_extract_top();
				priq->min_heap_insert_key(pt);
				disjset->update_elem(tid);
				
				flag = true;
				break;
			}
		}
	}

	while (!buffer.empty()) {
		pt = buffer.pop();
		priq->Min_Heap_Insert(pt);
	}

	return flag;
}

Framework::~Framework()
{
	delete[] cpu_freq_scale_space;
	delete[] cpu_latency_under_highest_freq;
	delete[] cpu_power_under_highest_freq;
	delete[] fpga_latency_under_highest_resusg;
	delete[] fpga_power_under_highest_resusg;
	delete[] transfer_data_size;

	if (task_chain)
		delete[] task_chain;

	if (dinic)
		delete dinic;
}
