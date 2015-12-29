#ifndef __PIPELATPOWER_H_
#define __PIPELATPOWER_H_

#include "PipeModel.h"
#include "Task.h"
#include "Bucket.h"

class PipeAreaPower : public PipeModel {
private:
	double system_power_cap;
	double area_constraint;

	// output
	double pipe_optthr;
	double pipe_area;
	double pipe_power;
	
public:
	PipeAreaPower();
	PipeAreaPower(double, double, int = 0, int = 0, bool = false);
	~PipeAreaPower();
//	void InitTaskChain(bool *ST);
	void BB();
	void DP();
	void DFS(std::vector<Task> &tv, int k, int n);
	void DFSOptThr(std::vector<Task> &tv, int k, int n);
	void Speak();
};

#endif
