#ifndef __PIPEMODEL_H_
#define __PIPEMODEL_H_

#include <vector>

#include "Task.h"
#include "LWTask.h"
#include "Bucket.h"

#define MAX_TASK_NUM 10

class PipeModel {
protected:
	// number of cores
	int num_cores_on_cpu;
	// number of tasks in a stream
	int num_tasks_in_stream;

	double bandwidth;
	// in bytes
	std::vector<int> transfer_data_size;

	// task chain
	std::vector<Task> task_chain;
	std::vector<LWTask> lwtask_chain;
	// use Bucket class to simulate pipeline structure
//	Bucket<Task> *bucket;
	Bucket *pipe;

//	bool is_solved;
	
public:
//	PipeModel();
	PipeModel(int = 0, int = 0, bool = false);
	~PipeModel();
	void ReadProfileConfig(char *fname);
	void PrintProfileConfig();
	void ClearProfileConfig();
	void get_bit_vector(bool *v, int i, int n);
	void Report();
	void ReportOpt();

	virtual void BB() = 0;
};

#endif
