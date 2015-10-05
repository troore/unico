
#include <cstdlib>
#include "Framework.h"

int main(int argc, char *argv[])
{
	Framework f;

	bool is_single_task_in_caps;

	f.read_profile_config();
	is_single_task_in_caps = f.init_predict_model();
	if (is_single_tasks_in_caps != 0) {
		if (is_single_tasks_in_caps == 1)
			std::cout << "Power constraints are too sringent!" << std::endl;
		else
			std::cout << "Latency constraints are too sringent!" << std::endl;
	}
	
	f.set_default_bounds();
	f.iterate();
	
    return 0;
}
