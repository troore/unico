#ifndef __TASK_H_
#define __TASK_H_

class Task {
	/*
	 * Index of *this* task in a stream.
	 */
	int id;
	/*
	 * Mapping result for *this* task, corresponding to the member
	 * 'v_map_result' in class Framework.
	 * 0: CPU
	 * 1: FPGA
	 */
	int type;
	double latency;
	double power;
	double lop;
public:
	Task();
	~Task();
	void set_latency(double l);
	void set_power(double p);
	void set_lop();
	double get_latency();
	double get_power();
	double get_lop();
};

#endif
