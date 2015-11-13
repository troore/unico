#ifndef __BUCKET_H_
#define __BUCKET_H_

#include "Task.h"

#define MAXS 10

struct Head {
	Task *ph;
	int n;
	double time;
	double power;
};

class Bucket {
private:
	Head head[MAXS];
	int ns;	// current number of stages
public:
	Bucket();
//	Bucket(int no);
	~Bucket();
	void insert_task(int id, Task *t);
	bool insert_bubble();
	void merge(int a, int b);
	void clear();
	double get_max_stage_len();
	double get_stage_len(int sno);
	double get_pipeline_throughput();
	double get_pipeline_latency();
	double get_pipeline_power();
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
