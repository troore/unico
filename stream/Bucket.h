#ifndef __BUCKET_H_
#define __BUCKET_H_

#include "Task.h"

#define MAXS 10

struct Head {
	Task *ph;
	int n; // number of tasks in current stage
	double lat;
	double power_cpu_base;
};

class Bucket {
private:
	Head head[MAXS];
	int ns;	// number of stages
//	double power_cpu_base;
	double power_fpga_base;
public:
	Bucket();
//	Bucket(int no);
	~Bucket();
	void insert_task(int sno, Task *t);
	bool insert_bubble();
	void merge_neib(int a, int b);
	void clear();
	void set_ns(int n);
	double get_max_stage_lat();
	double get_stage_lat(int sno);
	double get_pipeline_thr();
	double get_pipeline_lat();
	double get_pipeline_power();
	int get_pipeline_area();
	int get_sno_tid(int tid);
	void update_stage(int sno);
};

/*
template<class C> class Bucket {
private:
	C **head;
	int n;
public:
	Bucket();
	Bucket(int no);
	~Bucket();
	void insert(int id, C *t);
};
*/
#endif
