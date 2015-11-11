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
	void insert(int id, Task *t);
	void remove(int id);
	double get_pipeline_latency();
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
