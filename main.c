/**
 * @file main.c
 * @brief A random walk implementation conducted within the terminal.
 * @author Justin Thoreson
 */

#include "randomwalk.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

/**
 * @brief Information on how to run the random walk program.
 */
static const char* USAGE =
	"Usage: ./randomwalk <plane_width> <plane_height> <particle_count>";

/**
 * @brief Parse an 8-bit unsigned integer.
 * @param[in] arg The string argument to parse.
 * @param[out] value The parsed integer.
 * @return True if the integer is parsed successfully, false otherwise.
 */
static bool parse_uint8(const char* const arg, uint8_t* const value);

/**
 * @brief Parse command line arguments.
 * @param[out] args The parsed snake game arguments.
 * @param[in] argc The number of command line arguments.
 * @param[in] argv The command line arguments to parse.
 * @return True if the arguments are parsed successfully, false otherwise.
 */
static bool parse_args(
	randomwalk_args_t* const args,
	const int* const argc,
	char** const argv
);

int main(int argc, char** argv) {
	randomwalk_args_t args = { 0 };
	if (!parse_args(&args, &argc, argv)) {
		puts(USAGE);
		return 1;
	}
	(void)randomwalk(args);
	return 0;
}

static bool parse_uint8(const char* const arg, uint8_t* const value) {
	if (!arg || !value)
		return false;
	int64_t temp;
	if (!sscanf(arg, "%ld", &temp))
		return false;
	if (temp < 0 || temp > UINT8_MAX)
		return false;
	*value = (uint8_t)temp;
	return true;
}

static bool parse_args(
	randomwalk_args_t* const args,
	const int* const argc,
	char** const argv
) {
	if (*argc != 4)
		return false;
	if (!parse_uint8(argv[1], &args->width) ||
		!parse_uint8(argv[2], &args->height) ||
		!parse_uint8(argv[3], &args->num_particles))
		return false;
	return true;
}
