#ifndef __TASK_H_
#define __TASK_H_

#include <iostream>
#include <fstream>
#include <vector>

class Task {
	int id;	// index in the stream

	/*
	 * Attributes of a task
	 */

	// number of frequencies
	int num_freqs;
	// number of area candidates
	int num_areas;
	
	// vector of task latencies on CPU
	std::vector<double> v_lat_cpu;
	// vector of task powers on CPU (active power)
	std::vector<double> v_power_cpu;
	// vector of tasks latencies on FPGA
	std::vector<double> v_lat_fpga;
	// vector of task powers on FPGA (active power)
	std::vector<double> v_power_fpga;
	// vector of task areas on FPGA (number of slices for now)
	std::vector<int> v_area_fpga;

	/*
	 * mapping results
	 * */
	/*
	 * Mapping result for *this* task, corresponding to the member
	 * 'v_map_result' in class Framework.
	 * 0: CPU
	 * 1: FPGA
	 */
	int proc_type;

	// choice
	int cursor;
	// stage ID
	int sno;
	
public:
	Task();
	Task(const Task &T); // copy constructor
	Task &operator= (const Task &);
	~Task();
	
	void assign_attributes(std::fstream &fin);
	void print_attributes();
	
	void set_id(int i);
	void set_proc_type(int t);
	void set_cursor(int c);
	void set_sno(int no);
		
	int get_id();
	int get_num_freqs();
	int get_num_areas();
	int get_proc_type();
	double get_lat();
	double get_power();
	int get_area();
	int get_sno();
	int get_cursor();
	
	class Task *next;
};

#endif
