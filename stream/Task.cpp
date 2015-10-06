#include "Task.h"

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
