
#include <iostream>
#include <fstream>

#include "PipeModel.h"

/*
PipeModel::PipeModel()
{
}
*/

PipeModel::PipeModel(int nc, int nt, bool flag)
	: num_cores_on_cpu(nc), num_tasks_in_stream(nt)
{	
	pipe = new Bucket;
}

PipeModel::~PipeModel()
{
	if (pipe)
		delete pipe;
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

	fin >> num_tasks_in_stream;
//	task_chain = new Task[num_tasks_in_stream];

	/*
	 * profiling information for each task
	 */
	for (int i = 0; i < num_tasks_in_stream; i++) {
		Task t;
		LWTask lwt;
		
		t.set_id(i);
		t.set_proc_type(0); // CPU if unknown
		t.set_cursor(0); // most pessimistic >_<
		t.set_sno(0); // always put into the first pipeline stage at the beginning
		t.assign_attributes(fin);
		task_chain.push_back(t);

		lwt = t;
		lwtask_chain.push_back(lwt);
	}
	
	// bandwidth (MBps)
	bandwidth = 800;

	// vector of data size per frame between tasks (x4bytes, int)
//	transfer_data_size = new int[num_tasks_in_stream - 1];
	int size;
	for (int i = 0; i < (num_tasks_in_stream - 1); i++) {
		fin >> size;
		transfer_data_size.push_back(size);
	}

	fin.close();
}

void PipeModel::PrintProfileConfig()
{
	std::cout << "Number of CPU cores:\n" << num_cores_on_cpu << std::endl;
	
	std::cout << "Number of tasks in stream:\n" << num_tasks_in_stream << std::endl;

	for (int i = 0; i < num_tasks_in_stream; i++) {
		std::cout << "Task#" << i + 1 << ":\n";
		task_chain[i].print_attributes();
	}

	std::cout << "Bandwidth:\n" << bandwidth << std::endl;

	std::cout << "Transfer data size:\n";
	for (std::vector<int>::iterator it = transfer_data_size.begin(); it < transfer_data_size.end(); it++) {
		std::cout << *it;
		if (it != transfer_data_size.end())
			std::cout << " ";
	}
	std::cout << std::endl;
}

void PipeModel::ClearProfileConfig()
{
}

void PipeModel::get_bit_vector(bool *v, int i, int n)
{
	for (int k = 0; k < n; k++) {
		v[n - k - 1] = (i & 1);
		i >>= 1;
	}
}

void PipeModel::Report()
{
	/*
	if (!accepted) {
		std::cout << "Constraints are too rigid.\n";
		
		return;
	}
	*/
	std::cout << "#tid\t#sid\t#pid\t#cursor" << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++) {
		std::cout << i << "\t";
		std::cout << task_chain[i].get_sno() << "\t";
		std::cout << task_chain[i].get_proc_type() << "\t";
		std::cout << task_chain[i].get_cursor() << "\t";
		std::cout << "\n";
	}
}

void PipeModel::ReportOpt()
{
	std::cout << "#tid\t#sid\t#pid\t#cursor" << std::endl;
	for (int i = 0; i < num_tasks_in_stream; i++) {
		std::cout << i << "\t";
		std::cout << lwtask_chain[i].get_sno() << "\t";
		std::cout << lwtask_chain[i].get_proc_type() << "\t";
		std::cout << lwtask_chain[i].get_cursor() << "\t";
		std::cout << "\n";
	}
}
