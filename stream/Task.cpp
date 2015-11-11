#include "Task.h"

Task::Task()
{
}

Task::Task(const Task &T)
{
	type = T.type;
	time = T.time;
	power = T.power;
	freq = T.freq;
	next = T.next;
}

Task &Task::operator= (const Task &T)
{
	type = T.type;
	time = T.time;
	power = T.power;
	freq = T.freq;
	next = T.next;

	return *this;
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

void Task::set_freq(double f)
{
	freq = f;
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

double Task::get_freq()
{
	return freq;
}

Task::~Task()
{
}
