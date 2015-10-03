void PriorityQ::Exchange (int *x, int *y)
{
	*x = *x ^ *y;
	*y = *x ^ *y;
	*x = *x ^ *y;
}

/* the min heap part */
void PriorityQ::Min_Heapify (int i)
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
		if (l <= min_heap_size && min_heap[l] < min_heap[smallest])
			smallest = l;
		if (r <= min_heap_size && min_heap[r] < min_heap[smallest])
			smallest = r;
	}while (smallest != i);
}

int PriorityQ::Min_Heap_Extract_Top ()
{
/*	if (heap_size < 1)
	{
		cout << "heap underflow" << endl;
		exit (1);
	} */

	int min = min_heap[1];

	min_heap[1] = min_heap[min_heap_size];
	min_heap_size--;
	Min_Heapify (1);

	return min;
}

void PriorityQ::Min_Heap_Insert (int key)
{
	min_heap_size++;
	min_heap[min_heap_size] = key;

	int i = min_heap_size;
	while (i > 1 && min_heap[i] < min_heap[PARENT(i)])
	{
		Exchange (&min_heap[i], &min_heap[PARENT(i)]);
		i = PARENT(i);
	}
}

/* the max heap part */
void PriorityQ::Max_Heapify (int i)
{
	int l, r, largest;

	largest = -1;	// sentinel

	do
	{
		if (largest != -1)
		{
			Exchange(&max_heap[i], &max_heap[largest]);
			Exchange(&max_heap_idx[i], &max_heap_idx[largest]);
			i = largest;
		}

		l = LEFT(i);
		r = RIGHT(i);
		largest = i;
		if (l <= max_heap_size && max_heap[l] > max_heap[largest])
			largest = l;
		if (r <= max_heap_size && max_heap[r] > max_heap[largest])
			largest = r;
	} while (largest != i);
}

int PriorityQ::Max_Heap_Extract_Top ()
{
	int max = max_heap[1];

	max_heap[1] = max_heap[max_heap_size];
	max_heap_size--;
	Max_Heapify (1);

	return max;
}

void PriorityQ::Max_Heap_Insert (int key, int idx)
{
	max_heap_size++;
	max_heap[max_heap_size] = key;
	max_heap_idx[max_heap_size] = idx;

	int i = max_heap_size;
	while (i > 1 && max_heap[i] > max_heap[PARENT(i)])
	{
		Exchange (&max_heap[i], &max_heap[PARENT(i)]);
		Exchange (&max_heap_idx[i], &max_heap_idx[PARENT(i)]);
		i = PARENT(i);
	}
}

void PriorityQ::Heap_Init (int k)
{
	min_heap_size = max_heap_size = 0;
	Max_Heap_Insert(NEG_INFINITY, 0);
//	Min_Heap_Insert(POS_INFINITY, 0);
	for (int i = 1; i <= k; i++) {
		Max_Heap_Insert(key[i], idx[i]);
//		Min_Heap_Insert(key[i], idx[i]);
	}
}

void PriorityQ::read(int n)
{
	for (int i = 1; i <= n; i++) {
		cin >> key[i];
		idx[i] = i;
	}
}

int PriorityQ::Heap_Max_Key()
{
	return max_heap[0];
}

int PriorityQ::Heap_Max_Key_Id()
{
	return max_heap_idx[0];
}

void PriorityQ::Slide_Window(int n, int k)
{
	int max_heap_key, max_heap_key_id;

	max_heap_key = Heap_Max_Key();
	cout << max_heap_key;
	
	for (int i = k + 1; i <= n; i++) {
		cout << " ";
		max_heap_key_id = Heap_Max_Key_Id();
		while (max_heap_key_id < (i - k + 1)) {
			//	max_heap_key = Heap_Max_Key();
			max_heap_key = Max_Heap_Extract_Top();
			max_heap_key_id = Heap_Max_Key_Id();
		}
		Max_Heap_Insert(key[i], idx[i]);
		max_heap_key = Heap_Max_Key();
		cout << max_heap_key;
	}
}

int main (int argc, char *argv[])
{
	int n, k;

	PriorityQ p;
	
	while (cin >> n >> k) {
		p.read(n);
		p.Heap_Init(k);
		/*
		for (int i = 0; i < k; i++) {
			
		}
		for (int i = 0; i <= (n - k); i++) {
			
		}
		*/
		p.Slide_Window(n, k);
	}

	return 0;
}
