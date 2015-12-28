## Power Balance 

Of tasks executing on CPU, choose one to offload it onto FPGA, in order to save power.

Question: which one?

Spec:

1. Define a metric which quantizes the effect after a tasks is offloaded. This effect should take care of both power and latency. As this step is caused by the unsatisfaction of power cap, it should benefit power decrement best. On another hand, this offloading should bring about as little latency overhead as possible.

	LoP_{i}=LatencyIncrement_{i}/PowerDecrement_{i}
	LatencyIncrement_{i}=StreamLatency_{BeforeOffloading}-StreamLatency_{AfterOffloading}
	PowerDecrement_{i]=StreamPower_{BeforeOffloading}-StreamPower_{AfterOffloading}

Attention that the $StreamLatency$ is not $PipeLatency$, as:

	StreamLatency=\sum{TaskTime_{i}}
	PipeLatency=ns*max_pipe_stage_length

2. Choose the task with smallest $LoP$ to offload.

## Brute Force Search

1. The first hierarchy search

Function

Enumerate the number of pipeline stages.

Data Structures

Bit Vector 

Fields in this vector signify edges between tasks. So the length of this vector is $n-1$, where $n$ is the number of tasks. If the field is 0, tasks connected by this edge is in the same pipeline stage, vice versa.

Bucket

Pack the tasks in the same pipeline stage in a Bucket.

2. The second hierarchy search

Function

Map tasks in a pipeline stage onto CPU or FPGA. After dividing the tasks in a stream into pipeline, this step chooses a processor type for each task in a single stage.


Data Structures

Task

There is a Task object for each task in the stream. In a single Task, we should maintain its processor type, pipeline stage, and CPU frequency.

3. The third hierarchy search

Moreover, if a task is mapped onto CPU, we should choose a frequency for it. Similarly, if a task is mapped onto FPGA, we should choose an area for it. 

4. Branch pruning

+ Latency

When a task is assigned a frequency or an area, and it is the last task for a pipeline stage, if the latency of this pipeline stage will lead to the exceed of latency constraint, we can stop search for the remaining tasks.

+ Power

When a task is assigned a frequency or an area, and we manually assign the smallest frequency or area for the remaining tasks, if current power exceeds power constraint, we can stop searching for the remaining frequencies or areas for this task (if we search frequencie or area from small to big).

+ Area

Similar to power, when a task is assigned an area, and we manually assign the smallest areas for the remaining tasks, if current area execceds area constraint, we can stop searching for the remaining areas for this task (if we search areas from small to big).
