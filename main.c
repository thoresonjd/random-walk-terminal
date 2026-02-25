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
	"Usage: ./randomwalk <plane_width> <plane_height> <particle_count>"
	" <probability_direction_change> [delay_milliseconds]";

/**
 * @brief Parse an 8-bit unsigned integer.
 * @param[in] arg The string argument to parse.
 * @param[out] value The parsed integer.
 * @return True if the integer is parsed successfully, false otherwise.
 */
static bool parse_uint8(const char* const arg, uint8_t* const value);

/**
 * @brief Parse an 16-bit unsigned integer.
 * @param[in] arg The string argument to parse.
 * @param[out] value The parsed integer.
 * @return True if the integer is parsed successfully, false otherwise.
 */
static bool parse_uint16(const char* const arg, uint16_t* const value);

/**
 * @brief Parse command line arguments.
 * @param[out] args The parsed snake game arguments.
 * @param[in] argc The number of command line arguments.
 * @param[in] argv The command line arguments to parse.
 * @return True if the arguments are parsed successfully, false otherwise.
 */
static bool parse_args(
	randomwalk_args_t* const args,
	const int argc,
	char** const argv
);

/**
 * @brief Print the result of the random walk program.
 * @param[in] result The random walk result to print.
 */
static void print_randomwalk_result(const randomwalk_result_t result);

int main(int argc, char** argv) {
	randomwalk_args_t args = { 0 };
	if (!parse_args(&args, argc, argv)) {
		puts(USAGE);
		return 1;
	}
	print_randomwalk_result(randomwalk(args));
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

static bool parse_uint16(const char* const arg, uint16_t* const value) {
	if (!arg || !value)
		return false;
	int64_t temp;
	if (!sscanf(arg, "%ld", &temp))
		return false;
	if (temp < 0 || temp > UINT16_MAX)
		return false;
	*value = (uint16_t)temp;
	return true;
}

static bool parse_args(
	randomwalk_args_t* const args,
	const int argc,
	char** const argv
) {
	if (argc < 5)
		return false;
	if (!parse_uint8(argv[1], &args->width) ||
		!parse_uint8(argv[2], &args->height) ||
		!parse_uint8(argv[3], &args->num_particles) ||
		!parse_uint8(argv[4], &args->prob_dir_change))
		return false;
	if (argc == 6)
		(void)parse_uint16(argv[5], &args->delay_ms);
	return true;
}

static void print_randomwalk_result(const randomwalk_result_t result) {
	switch(result) {
		case RANDOMWALK_OK:
			printf("RANDOMWALK_OK (%d)\n", RANDOMWALK_OK);
			break;
		case RANDOMWALK_DONE:
			printf("RANDOMWALK_DONE (%d)\n", RANDOMWALK_DONE);
			break;
		case RANDOMWALK_FAIL:
			printf("RANDOMWALK_FAIL (%d)\n", RANDOMWALK_FAIL);
			break;
		default:
			puts("Unknown result");
	};
}

