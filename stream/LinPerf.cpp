
#include <iostream>
#include <cmath>

#include "LinPerf.h"

LinPerf::LinPerf()
{
	lin_lat_opt = 0.0;
	lin_eng_opt = 0.0;
}

void LinPerf::BB()
{
//	std::cout << "This is LinPerf's BB()" << std::endl;

	bool *v = new bool[num_tasks_in_stream];

	int bound = (int)pow(2.0, num_tasks_in_stream);

	lin_lat_opt = -1.0;

	for (int i = 0; i < num_tasks_in_stream; i++) {
		pipe->insert_task(0, &task_chain[i]);
	}
	pipe->set_ns(1);
	
	for (int i = 0; i < bound; i++) {
		get_bit_vector(v, i, num_tasks_in_stream);
		for (int j = 0; j < num_tasks_in_stream; j++) {
			task_chain[j].set_proc_type(v[j]);
			/*
			 * Don't account for frequency scaling on CPU
			 * and module selection on FPGA in static scheduling process.
			 */
			//	task_chain[j].set_cursor(0);
			if (v[j] == 0) {
				// CPU
				task_chain[j].set_cursor(task_chain[j].get_num_freqs() - 1);
			}
			else {
				task_chain[j].set_cursor(task_chain[j].get_num_areas() - 1);
			}
			pipe->update_stage(pipe->get_sno_tid(j));
		}

		if (lin_lat_opt < 0 || lin_lat_opt > pipe->get_pipeline_lat()) {
			lin_lat_opt = pipe->get_pipeline_lat();
			lin_eng_opt = pipe->get_pipeline_eng();
			for (int j = 0; j < num_tasks_in_stream; j++) {
				lwtask_chain[j] = task_chain[j];
			}
		}
	}

	ReportOpt();

	delete [] v;
}

void LinPerf::ReportOpt()
{
	std::cout << "LatOpt:\t" << lin_lat_opt << " ms" << std::endl;
	std::cout << "EnergyOpt:\t" << lin_eng_opt << " J" << std::endl;

	std::cout << "#tid\t#sid\t#pid\t#cursor" << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++) {
		std::cout << i << "\t";
		std::cout << lwtask_chain[i].get_sno() << "\t";
		std::cout << lwtask_chain[i].get_proc_type() << "\t";
		std::cout << lwtask_chain[i].get_cursor() << "\t";
		std::cout << "\n";
	}
}

LinPerf::~LinPerf()
{
}
