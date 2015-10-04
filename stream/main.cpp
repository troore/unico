
#include "Framework.h"

int main(int argc, char *argv[])
{
	Framework f;

	f.read_profile_config();
	f.init_predict_model();
	f.iterate();
	
    return 0;
}
