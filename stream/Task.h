#ifndef __TASK_H_
#define __TASK_H_

class Task {
	/*
	 * Mapping result for *this* task, corresponding to the member
	 * 'v_map_result' in class Framework.
	 * 0: CPU
	 * 1: FPGA
	 */
	int type;
	double time;
	double power;
	// if type = 1, frequency = 0
	double freq;
	// stage ID
	int sno;
public:
	Task();
	Task(const Task &T); // copy constructor
	Task &operator= (const Task &);
	~Task();
	void set_type(int t);
	void set_time(double t);
	void set_power(double p);
	void set_sno(int no);
	void set_freq(double f);
	int get_type();
	double get_time();
	double get_power();
	int get_sno();
	double get_freq();

	class Task *next;
};

#endif
