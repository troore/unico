
#include <algorithm>
#include <cstdlib>
#include "Bucket.h"
#include "Task.h"

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
	head[id].power = (head[id].power * head[id].time + t->get_power() * t->get_lat()) /
		(head[id].time + t->get_lat());
	head[id].time = head[id].time + t->get_lat();
}

bool Bucket::insert_bubble()
{
	/*
	 * find the stage with smallest execution time,
	 * its index is _sno_.
	 */
	int sno = 0;

	for (int i = 1; i < ns; i++) {
		if (head[i].time < head[sno].time)
			sno = i;
	}

	/*
	 * find the _sno_th neighbors, and choose one among them
	 * with smaller stage length
	 */
	int sno_above_neb = -1, sno_below_neb = -1;
	int sno_min_neb;

	if ((sno - 1) >= 0)
		sno_above_neb = sno - 1;
	if ((sno + 1) < ns)
		sno_below_neb = sno + 1;

	if (sno_above_neb >= 0 && sno_above_neb >= 0) {
		if (head[sno_above_neb].time < head[sno_above_neb].time)
			sno_min_neb = sno_above_neb;
		else
			sno_min_neb = sno_below_neb;
	}
	else if (sno_above_neb >= 0)
		sno_min_neb = sno_above_neb;
	else if (sno_below_neb >= 0)
		sno_min_neb = sno_below_neb;
	else
		return false;

	// do the merge
	merge(sno, sno_min_neb);

	// squeeze vacant spages to compact pipeline
	
	return true;
}

void Bucket::merge(int a, int b)
{
	Task *p, *q;
	
	// guarantee that a < b
	if (a > b) {
		std::swap(a, b);
	}
	// find tail of stage _b_
	p = head[b].ph;
	while (p->next != NULL)
		p = p->next;
	// find head of stage _a_
	q = head[a].ph;
	head[a].ph = head[b].ph;
	p->next = q;
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

void Bucket::set_ns(int n)
{
	ns = n;
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
