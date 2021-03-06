
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lte_phy.h"
#include "check/check.h"

int num_threads;

void check(char *tx_in_fname, char *rx_out_fname)
{
	int err_n;
		
	err_n = check_float(tx_in_fname, rx_out_fname);
	printf("%d\n", err_n);
}


double gflop_counter(LTE_PHY_PARAMS *lte_phy_params)
{
	int cnter = 0;

	return 1.0e-9 * cnter;
}

