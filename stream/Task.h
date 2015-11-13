#ifndef __TASK_H_
#define __TASK_H_

class Task {
	int id;	// index in the stream
	/*
	 * Mapping result for *this* task, corresponding to the member
	 * 'v_map_result' in class Framework.
	 * 0: CPU
	 * 1: FPGA
	 */
	int type;
	double time;
	double power;
	double cpu_freq;
	int cpu_freq_cursor;
	// stage ID
	int sno;
	/*
	 * performance and power models
	 * \chi: performance
	 * \lambda: power
	 */
	double chi;
	double lambda;
public:
	Task();
	Task(const Task &T); // copy constructor
	Task &operator= (const Task &);
	~Task();
	void set_id(int i);
	void set_type(int t);
	void set_time(double t);
	void set_power(double p);
	void set_sno(int no);
	void set_cpu_freq(double f);
	void set_cpu_freq_cursor(int c);
	void init_perf_model();
	void init_power_model();
		
	int get_id();
	int get_type();
	double get_time();
	double get_power();
	int get_sno();
	double get_cpu_freq();
	int get_cpu_freq_cursor();
	double get_time_at_cpu_freq(double freq);
	double get_power_at_cpu_freq(double freq);

	class Task *next;
};

#endif
