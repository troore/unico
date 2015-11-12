#include "Task.h"

Task::Task()
{
}

Task::Task(const Task &T)
{
	id = T.id;
	type = T.type;
	time = T.time;
	power = T.power;
	cpu_freq = T.cpu_freq;
	cpu_freq_cursor = T.cpu_freq_cursor;
	next = T.next;
}

Task &Task::operator= (const Task &T)
{
	id = T.id;
	type = T.type;
	time = T.time;
	power = T.power;
	cpu_freq = T.cpu_freq;
	cpu_freq_cursor = T.cpu_freq_cursor;
	next = T.next;

	return *this;
}

void Task::set_id(int i)
{
	id = i;
}

void Task::set_type(int t)
{
	type = t;
}

void Task::set_time(double t)
{
	time = t;
}

void Task::set_power(double p)
{
	power = p;
}

void Task::set_sno(int no)
{
	sno = no;
}

void Task::set_cpu_freq(double f)
{
	cpu_freq = f;
}

void Task::set_cpu_freq_cursor(int c)
{
	cpu_freq_cursor = c;
}

int Task::get_id()
{
	return id;
}

int Task::get_type()
{
	return type;
}

double Task::get_time()
{
	return time;
}

double Task::get_power()
{
	return power;
}

int Task::get_sno()
{
	return sno;
}

double Task::get_cpu_freq()
{
	return cpu_freq;
}

int Task::get_cpu_freq_cursor()
{
	return cpu_freq_cursor;
}

double Task::get_time_at_cpu_freq(double freq)
{
	return 0.0;
}

double Task::get_power_at_cpu_freq(double freq)
{
	return 0.0;
}

Task::~Task()
{
}
