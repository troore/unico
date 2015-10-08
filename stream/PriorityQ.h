
#ifndef __PRIORITYQ_H_
#define __PRIORITYQ_H_

#include "Task.h"

#define PARENT(i)	(i / 2) 
#define LEFT(i)		(i * 2)
#define RIGHT(i)	(i * 2 + 1)

#define NEG_INFINITY	(0x80000000)
#define POS_INFINITY	(0x7FFFFFFF)

class PriorityQ {
	int len;
	int min_heap_size, max_heap_size;
	Task **min_task_heap, **max_task_heap;
public:
	PriorityQ(int n);
	PriorityQ(const PriorityQ &Q); // copy constructor
	~PriorityQ();

	PriorityQ &operator= (const PriorityQ &);
	
	void exchange(Task *x, Task *y);

	void build_min_heap(Task *);
	void max_heap_init();
	void max_heapify(int i);
	Task *max_heap_extract_top();
	void max_heap_insert(Task *key);
	Task *heap_max_key();
	int heap_max_key_Id();
	void min_heap_replace_with_key(int idx, Task *key);
	void min_heap_delete_key(int idx);

	void build_min_heap(Task *);
	void min_heap_init();
	void min_heapify(int i);
	Task *min_heap_extract_top();
	void min_heap_insert(Task *key);
	Task *heap_min_key();
	int heap_min_key_id();
	void min_heap_replace_with_key(int idx, Task *key);
	void min_heap_delete_key(int idx);
};

#endif
