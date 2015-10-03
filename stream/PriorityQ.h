#ifndef __PRIORITYQ_H_
#define __PRIORITYQ_H_

#define N (1000000 + 2)

#define PARENT(i)	(i / 2) 
#define LEFT(i)		(i * 2)
#define RIGHT(i)	(i * 2 + 1)

#define NEG_INFINITY	(0x80000000)
#define POS_INFINITY	(0x7FFFFFFF)

class PriorityQ {
	int key[N];
	int idx[N];

	int min_heap_size, max_heap_size;
	int min_heap[N], max_heap[N];
	int min_heap_idx[N], max_heap_idx[N];
public:
	void read(int n);
	void Exchange(int *x, int *y);
	void Max_Heapify(int i);
	int Max_Heap_Extract_Top();
	void Max_Heap_Insert (int key, int idx);
	void Heap_Init (int k);
	int Heap_Max_Key();
	int Heap_Max_Key_Id();

	void Min_Heapify (int i);
	int Min_Heap_Extract_Top ();
	void Min_Heap_Insert (int key);

	void Slide_Window(int n, int k);
};


#endif
