
#include <algorithm>
#include <cstdlib>
#include "Bucket.h"
#include "Task.h"

/*
 * Pipeline structure
 *
 * Task chain:
 *   0    1    2    3    4
 * --------------------------
 * | t0 | t1 | t2 | t3 | t4 |
 * --------------------------
 *
 * Bucket:
 *	 ------
 * 3 | t4 |
 *   -----------
 * 2 | t2 | t3 |
 *   -----------
 * 1 | t1 |
 *   ------
 * 0 | t0 |
 *   ------
 *
 * */

Bucket::Bucket()
{
	ns = 0;
	for (int i = 0; i < MAXS; i++) {
		head[i].lat = 0.0;
		head[i].power_cpu_base = 4.4; // 4.4 Watt is from measurement
		head[i].ph = NULL;
		head[i].n = 0;
	}
//	power_cpu_base = 4.4; // 4.4 Watt is from measurement
	power_fpga_base = 0.120; // from measurement too
//	power_cpu_base = 0.0; // 4.4 Watt is from measurement
//	power_fpga_base = 0.0; // from measurement too
}

Bucket::~Bucket()
{
}

/*
 * Insert a task "t" into a pipeline stage indexed by "sno". 
 */
void Bucket::insert_task(int sno, Task *t)
{
	Task *tmp;

	// allocate a new stage
	if (head[sno].ph == NULL)
		ns++;

	// insert _t_ into the _id_th stage 
	tmp = head[sno].ph;
	head[sno].ph = t;
	t->next = tmp;

	// update stage latency
	head[sno].lat = head[sno].lat + t->get_lat();
}

/*
 * Actually, "bubble" is borrowed from CPU pipeline.
 * When inserting a bubble, we pick two pipeline stages 
 * and merge them according to some metrics.
 */
bool Bucket::insert_bubble()
{
	/*
	 * find the stage with smallest execution time,
	 * its index is _sno_.
	 */
	int sno = 0;

	for (int i = 1; i < ns; i++) {
		if (head[i].lat < head[sno].lat)
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
		if (head[sno_above_neb].lat < head[sno_above_neb].lat)
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
	merge_neib(sno, sno_min_neb);

	// squeeze vacant spages to compact pipeline
	
	return true;
}

/*
 * Merge two pipeline stages in neighbour, and adjust 
 * pipeline structure.
 * */
void Bucket::merge_neib(int a, int b)
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
	// insert _a_ before _b_
	head[a].ph = head[b].ph;
	p->next = q;

	/*
	 * Adjust pipeline structure: move each stage backward for 
	 * one step.
	 * */
	for (int i = a + 1; i <= ns; i++) {
		head[i - 1] = head[i];
	}

	ns--;
}

void Bucket::clear()
{
	for (int i = 0; i < ns; i++) {
		head[i].lat = 0.0;
		head[i].ph = NULL;
		head[i].n = 0;
	}
}

void Bucket::set_ns(int n)
{
	ns = n;
}

double Bucket::get_max_stage_lat()
{
	double max_stage_lat;

	max_stage_lat = -1.0;
	for (int i = 0; i < ns; i++) {
		if (head[i].ph != NULL) {
			// if the _i_th stage is not vacant
			if (get_stage_lat(i) - max_stage_lat > 1e-3)
				max_stage_lat = head[i].lat;
		}
	}

	return max_stage_lat;
}

double Bucket::get_stage_lat(int sno)
{
	return head[sno].lat;
}

double Bucket::get_pipeline_lat()
{
	return (get_max_stage_lat() * ns);
}

double Bucket::get_pipeline_thr()
{
	return 1.0 / get_max_stage_lat();
}

double Bucket::get_pipeline_power()
{
	double energy = 0.0, power = 0.0;

	for (int i = 0; i < ns; i++) {
		Task *p = head[i].ph;

		while (p) {
			energy += (p->get_lat() * p->get_power());
			p = p->next;
		}
	}

	// active
	power = energy / get_max_stage_lat();

	for (int i = 0; i < ns; i++) {
		power += head[i].power_cpu_base;
	}
//	power += power_cpu_base;
	power += power_fpga_base;

	return power;
}

int Bucket::get_pipeline_area()
{
	int area = 0;

	for (int i = 0; i < ns; i++) {
		Task *p = head[i].ph;

		while (p) {
			area += p->get_area();
			p = p->next;
		}
	}

	return area;
}

/*
 * Get stage number of a task.
 * */
int Bucket::get_sno_tid(int tid)
{
	for (int i = 0; i < ns; i++) {
		Task *p = head[i].ph;

		while (p) {
			if (p->get_id() == tid)
				return i;
			p = p->next;
		}
	}

	// you should never get here, error
	return -1;
}

/*
 * Update stage information, e.g., latency, as neccesary. 
 * It may be caused by task's update in this stage.
 * */
void Bucket::update_stage(int sno)
{
	Task *p = head[sno].ph;

	head[sno].lat = 0.0;
	while (p) {
		head[sno].lat += p->get_lat();
		p = p->next;
	}
}
