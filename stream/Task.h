#ifndef __TASK_H_
#define __TASK_H_

class Task {
	double latency;
	double power;
	double lop;
public:
	void set_latency(double l);
	void set_power(double p);
	void set_lop();
	double get_latency();
	double get_power();
	double get_lop();
};

#endif
