/**
 * @file main.c
 * @brief A random walk implementation conducted within the terminal.
 * @author Justin Thoreson
 */

#include "randomwalk.h"

int main(void) {
	randomwalk_args_t args;
	args.width = 200;
	args.height = 200;
	args.num_particles = 10;
	(void)randomwalk(args);
	return 0;
}
