
#ifndef __LWTASK_H_
#define __LWTASK_H_

#include "Task.h"

/*
 * A __L__ight__W__eighted Task, used to record the cursor each task chooses.
 */
class LWTask {
private:
	int id;
	int proc_type;
	int cursor;
	int sno;
	
public:
	LWTask();
	~LWTask();

	LWTask &operator= (Task &T);

	void set_id(int );
	void set_proc_type(int );
	void set_cursor(int );
	void set_sno(int );
	
	int get_id();
	int get_proc_type();
	int get_cursor();
	int get_sno();
};

#endif
