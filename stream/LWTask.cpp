#include "LWTask.h"

LWTask::LWTask()
{
	id = -1;
	proc_type = -1;
	cursor = -1;
	sno = -1;
}

LWTask::~LWTask()
{
}

LWTask &LWTask::operator= (Task &T)
{
	id = T.get_id();
	proc_type = T.get_proc_type();
	cursor = T.get_cursor();
	sno = T.get_sno();
	
	return *this;
}

void LWTask::set_id(int i)
{
	id = i;
}

void LWTask::set_proc_type(int t)
{
	proc_type = t;
}

void LWTask::set_sno(int no)
{
	sno = no;
}

void LWTask::set_cursor(int c)
{
	cursor = c;
}

int LWTask::get_id()
{
	return id;
}

int LWTask::get_proc_type()
{
	return proc_type;
}

int LWTask::get_sno()
{
	return sno;
}

int LWTask::get_cursor()
{
	return cursor;
}
