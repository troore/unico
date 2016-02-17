
#include "Task.h"

Task::Task()
{
}

Task::~Task()
{
}

Task::Task(const Task &T)
{
	*this = T;
}

Task &Task::operator= (const Task &T)
{
	id = T.id;
	num_freqs = T.num_freqs;
	num_areas = T.num_areas;
	v_lat_cpu = T.v_lat_cpu;
	v_power_cpu = T.v_power_cpu;
	v_lat_fpga = T.v_lat_fpga;
	v_power_fpga = T.v_power_fpga;
	v_area_fpga = T.v_area_fpga;

	proc_type = T.proc_type;
	cursor = T.cursor;
	sno = T.sno;
	next = T.next;
	
	return *this;
}

void Task::set_id(int i)
{
	id = i;
}

void Task::set_proc_type(int t)
{
	proc_type = t;
}

void Task::set_sno(int no)
{
	sno = no;
}

void Task::set_cursor(int c)
{
	cursor = c;
}

int Task::get_id()
{
	return id;
}

int Task::get_num_freqs()
{
	return num_freqs;
}

int Task::get_num_areas()
{
	return num_areas;
}

int Task::get_proc_type()
{
	return proc_type;
}

int Task::get_sno()
{
	return sno;
}

int Task::get_cursor()
{
	return cursor;
}

double Task::get_lat()
{
	return (1.0 - proc_type) * v_lat_cpu[cursor] + (double)proc_type * v_lat_fpga[cursor];
}

double Task::get_power()
{
	return (1.0 - proc_type) * v_power_cpu[cursor] + (double)proc_type * v_power_fpga[cursor];
}

double Task::get_eng()
{
	return get_power() * get_lat();
}

int Task::get_area()
{
	return proc_type * v_area_fpga[cursor];
}

void Task::assign_attributes(std::fstream &fin)
{
	double lat_cpu, power_cpu, lat_fpga, power_fpga, area_fpga;
	
	fin >> num_freqs;
	for (int i = 0; i < num_freqs; i++) {
		fin >> lat_cpu;
		v_lat_cpu.push_back(lat_cpu);
	}
	for (int i = 0; i < num_freqs; i++) {
		fin >> power_cpu;
		v_power_cpu.push_back(power_cpu - 4.4);
	//	v_power_cpu.push_back(power_cpu);
	}
	
	fin >> num_areas;
	for (int i = 0; i < num_areas; i++) {
		fin >> lat_fpga;
		v_lat_fpga.push_back(lat_fpga);
	}
	for (int i = 0; i < num_areas; i++) {
		fin >> power_fpga;
		v_power_fpga.push_back(power_fpga);
	}
	for (int i = 0; i < num_areas; i++) {
		fin >> area_fpga;
		v_area_fpga.push_back(area_fpga);
	}
}

void Task::print_attributes()
{
	std::cout << "Number of frequencies:\n" << num_freqs << std::endl;
	for (int j = 0; j < num_freqs; j++) {
		std::cout << v_lat_cpu[j];
		if (j != (num_freqs - 1))
			std::cout << " ";
		else
			std::cout << std::endl;
	}
	for (int j = 0; j < num_freqs; j++) {
		std::cout << v_power_cpu[j];
		if (j != (num_freqs - 1))
			std::cout << " ";
		else
			std::cout << std::endl;
	}
	
	std::cout << "Number of areas:\n" << num_areas << std::endl;
	for (int j = 0; j < num_areas; j++) {
		std::cout << v_lat_fpga[j];
		if (j != (num_areas - 1))
			std::cout << " ";
		else
			std::cout << std::endl;
	}
	for (int j = 0; j < num_areas; j++) {
		std::cout << v_power_fpga[j];
		if (j != (num_areas - 1))
			std::cout << " ";
		else
			std::cout << std::endl;
	}
	for (int j = 0; j < num_areas; j++) {
		std::cout << v_area_fpga[j];
		if (j != (num_areas - 1))
			std::cout << " ";
		else
			std::cout << std::endl;
	}
}
