
#include <cstdlib>
#include "Bucket.h"
#include "Task.h"

/*
template<class C> Bucket<C>::Bucket()
{}

template<class C> Bucket<C>::Bucket(int no)
	: n(no)
{
	head = new C *[n];

	for (int i = 0; i < n; i++)
		head[i] = NULL;
}

template<class C> void Bucket<C>::insert(int id, C *t)
{
	C *tmp = head[id];
	head[id] = t;
	t->next = tmp;
}

template<class C> Bucket<C>::~Bucket()
{
	if (head)
		delete [] head;
}
*/

Bucket::Bucket()
{
	ns = 0;
	for (int i = 0; i < MAXS; i++) {
		head[i].time = 0.0;
		head[i].power = 0.0;
		head[i].ph = NULL;
		head[i].n = 0;
	}
}

void Bucket::insert_task(int id, Task *t)
{
	Task *tmp;

	// allocate a new stage
	if (head[id].ph == NULL)
		ns++;

	// insert _t_ into the _id_th stage 
	tmp = head[id].ph;
	head[id].ph = t;
	t->next = tmp;
	// update stage time and power
	// TODO: consider the idle or base power on CPU and FPGA
	head[id].power = (head[id].power * head[id].time + t->get_power() * t->get_time()) /
		(head[id].time + t->get_time());
	head[id].time = head[id].time + t->get_time();
}

bool Bucket::insert_bubble()
{
	return true;
}

void Bucket::clear()
{
	for (int i = 0; i < ns; i++) {
		head[i].time = 0.0;
		head[i].power = 0.0;
		head[i].ph = NULL;
		head[i].n = 0;
	}
}

double Bucket::get_max_stage_len()
{
	double max_stage_len;

	max_stage_len = -1.0;
	for (int i = 0; i < ns; i++) {
		if (head[i].ph != NULL) {
			// if the _i_th stage is not vacant
			if (head[i].time - max_stage_len > 1e-3)
				max_stage_len = head[i].time;
		}
	}

	return max_stage_len;
}

double Bucket::get_stage_len(int sno)
{
	return head[sno].time;
}

double Bucket::get_pipeline_latency()
{
	return (get_max_stage_len() * ns);
}

double Bucket::get_pipeline_throughput()
{
	return 1.0 / get_max_stage_len();
}

double Bucket::get_pipeline_power()
{
	double power = 0.0;

	for (int i = 0; i < ns; i++) {
		power += head[i].power;
	}

	return power;
}

Bucket::~Bucket()
{
}
