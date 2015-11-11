
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

void Bucket::insert(int id, Task *t)
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

void Bucket::remove(int id)
{
}

double Bucket::get_pipeline_latency()
{
	double max_stage_len;
	int n;

	max_stage_len = -1.0;
	n = 0;
	for (int i = 0; i < MAXS; i++) {
		if (head[i].ph != NULL) {
			n++;
			// if the _i_th stage is not vacant
			if (head[i].time - max_stage_len > 1e-3)
				max_stage_len = head[i].time;
		}
	}

	return (max_stage_len * n);
}

Bucket::~Bucket()
{
}
