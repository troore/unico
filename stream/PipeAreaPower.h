#ifndef __PIPELATPOWER_H_
#define __PIPELATPOWER_H_

#include "PipeModel.h"
#include "Task.h"
#include "Bucket.h"

class PipeAreaPower : public PipeModel {
private:
	double system_power_cap;
	double area_constraint;
	
public:
	PipeAreaPower(double pc, double ac);
	~PipeAreaPower();
//	void InitTaskChain(bool *ST);
	void BB();
	void DP();
	void DFS(std::vector<Task> &tv, int k, int n);
};

#endif
