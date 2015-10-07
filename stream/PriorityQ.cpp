
#include "PriorityQ.h"

PriorityQ::PriorityQ(int n)
	: len(n)
{
	min_task_heap = new (Task *)[n];
	max_task_heap = new (Task *)[n];
}

void PriorityQ::exchange (Task **x, Task **y)
{
	*x = *x ^ *y;
	*y = *x ^ *y;
	*x = *x ^ *y;
}

/*
 * the min heap part
 */
void PriorityQ::min_heap_init()
{
	min_heap_size = 0;
}

void PriorityQ::build_min_heap(Task *task_chain)
{
	for (int i = 0; i < n; i++) {
		min_heap_insert(&task_chain[i]);
	}
}

void PriorityQ::min_heapify (int i)
{
	int l, r, smallest;

	smallest = -1;	// sentinel
	do
	{
		if (smallest != -1)
		{
			Exchange (&min_heap[i], &min_heap[smallest]);
			i = smallest;
		}

		l = LEFT(i);
		r = RIGHT(i);
		smallest = i;
		if (l <= min_heap_size &&
			min_heap[l]->lop < min_heap[smallest]->lop)
			smallest = l;
		if (r <= min_heap_size &&
			min_heap[r]->lop < min_heap[smallest]->lop)
			smallest = r;
	}while (smallest != i);
}

Task *PriorityQ::min_heap_extract_top()
{
	if (min_heap_size < 1) {
		cout << "heap underflow" << endl;
		exit (1);
	}

	Task *min = min_heap[1];

	min_heap[1] = min_heap[min_heap_size];
	min_heap_size--;
	min_heapify(1);

	return min;
}

void PriorityQ::min_heap_insert(Task *key)
{
	min_heap_size++;
	min_heap[min_heap_size] = key;

	int i = min_heap_size;
	
	while (i > 1 && min_heap[i]->lop < min_heap[PARENT(i)]->lop) {
		exchange(&min_heap[i], &min_heap[PARENT(i)]);
		i = PARENT(i);
	}
}

void PriorityQ::min_heap_replace_with_key(int idx, Task *key)
{
	min_heap_delete_key(idx);
	min_heap_insert(key);
}

void PriorityQ::min_heap_delete_key(int idx)
{
	int i = idx;
	
	min_task_heap[i]->lop = NEG_INFINITY;
	
	while (i > 1) {
		exchange(&min_heap[i], &min_heap[PARENT(i)]);
		i = PARENT(i);
	}

	min_heap_extract_top();
}

Task *PriorityQ::heap_min_key()
{
	return min_task_heap[1];
}

int PriorityQ::heap_min_key_id()
{
	return min_task_heap[1]->id;
}

/*
 * the max heap part
 */
void PriorityQ::min_heap_init()
{
	min_heap_size = 0;
}

void PriorityQ::build_max_heap(Task *task_chain)
{
	for (int i = 0; i < n; i++) {
		max_heap_insert(&task_chain[i]);
	}
}

void PriorityQ::max_heapify (int i)
{
	int l, r, largest;

	largest = -1;	// sentinel

	do
	{
		if (largest != -1)
		{
			exchange(&max_heap[i], &max_heap[largest]);
			i = largest;
		}

		l = LEFT(i);
		r = RIGHT(i);
		largest = i;
		if (l <= max_heap_size &&
			max_task_heap[l]->lop > max_task_heap[largest]->lop)
			largest = l;
		if (r <= max_heap_size &&
			max_task_heap[r]->lop > max_task_heap[largest]->lop)
			largest = r;
	} while (largest != i);
}

Task *PriorityQ::max_heap_extract_top()
{
	Task *max = max_heap[1];

	max_heap[1] = max_heap[max_heap_size];
	max_heap_idx[1] = max_heap_idx[max_heap_size];
	max_heap_size--;
	max_heapify(1);

	return max;
}

void PriorityQ::max_heap_insert(Task *key)
{
	max_heap_size++;
	max_heap[max_heap_size] = key;

	int i = max_heap_size;
	
	while (i > 1 && max_task_heap[i]->lop > max_task_heap[PARENT(i)]->lop) {
		exchange(&max_task_heap[i], &max_task_heap[PARENT(i)]);
		i = PARENT(i);
	}
}

Task *PriorityQ::heap_max_key()
{
	return max_heap[1];
}

int PriorityQ::heap_max_key_id()
{
	return max_heap_idx[1]->id;
}

void PriorityQ::max_heap_replace_with_key(int idx, Task *key)
{
	max_heap_delete_key(idx);
	max_heap_insert(key);
}

void PriorityQ::max_heap_delete_key(int idx)
{
	int i = idx;
	
	max_task_heap[i]->lop = POS_INFINITY;
	
	while (i > 1) {
		exchange(&max_heap[i], &max_heap[PARENT(i)]);
		i = PARENT(i);
	}

	max_heap_extract_top();
}

PriorityQ::~PriorityQ()
{
	delete[] min_task_heap;
	delete[] max_task_heap;
}

