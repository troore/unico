#include "Task.h"

void Task::Task()
{
}

void Task::Task(const Task &T)
{
	id = T.id;
	type = T.type;
	latency = T.latency;
	power = T.power;
	lop = T.lop;
}

void Task::Task(int x, int t)
	: id(x), type(t)
{
}

void Task::set_id(int x)
{
	id = x;
}

void Task::set_latency(double l)
{
	latency = l;
}

void Task::set_power(double p)
{
	power = p;
}

void Task::set_lop()
{
	lop = latency / power;
}

int Task::get_id()
{
	return id;
}

double Task::get_latency()
{
	return latency;
}

double Task::get_power()
{
	return power;
}

double Tasks::get_lop()
{
	return lop;
}

void Task::~Task()
{
}
