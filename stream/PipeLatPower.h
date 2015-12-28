#ifndef __PIPELATPOWER_H_
#define __PIPELATPOWER_H_

#include "PipeModel.h"
#include "Task.h"
#include "Bucket.h"

class PipeLatPower : public PipeModel {
private:
	double system_power_cap;
	double latency_constraint;
	
	double latency_lower_bound;
	double throughput_lower_bound;
	double power_lower_bound;
	
	// output
	double time;
	double power;

	// interation result
	bool accepted;
public:
	PipeLatPower(double pc, double lc);
	~PipeLatPower();
	void InitTaskChain(bool *ST);
	void BranchBound();
	void Heuristic();
	bool PowerBalance(bool *ST);
	bool FreqScaling();
	void DFSFreqSpace(std::vector<Task *>tv, int k, int n);
	void Speak();
};

#endif
