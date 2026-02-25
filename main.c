/**
 * @file main.c
 * @brief A random walk implementation conducted within the terminal.
 * @author Justin Thoreson
 */

#include "randomwalk.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/**
 * @brief Information on how to run the random walk program.
 */
static const char* USAGE =
	"Usage: ./randomwalk [arguments]\n"
	"Parameters (R = required | O = optional):\n"
	"[R] --width=<uint8>           width of the plane\n"
	"[R] --height=<uint8>          height of the plane\n"
	"[R] --pcount=<uint8>          initial particle count\n"
	"[O] --prob-dir-change={0-100} probability a particle changes direction\n"
	"[O] --delay=<uint16>          delay between frames in milliseconds";

/**
 * @brief Move a string pointer forward passed a specified prefix.
 * @param[in,out] string The string in which the prefix is skipped.
 * @param[in] prefix The prefix to skip.
 * @return True if skipping succeeded, false otherwise.
 */
static bool skip_prefix(char** string, const char* const prefix);

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
 * @brief Parse a single command line argument.
 * @param[out] args The parsed random walk argument.
 * @param[in] arg The argument to parse.
 * @return True if parsing succeeded, false otherwise.
 */
static bool parse_arg(randomwalk_args_t* const args, char* arg);

/**
 * @brief Parse command line arguments.
 * @param[out] args The parsed random walk arguments.
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
 * @brief Verify that all required arguments have been provided.
 *
 * Required arguments are those that must have non-zero values.
 *
 * @param[in] args The random walk arguments.
 * @return True if all required arguments are set, false otherwise.
 */
static bool validate_required_args(const randomwalk_args_t* const args);

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

static bool skip_prefix(char** string, const char* const prefix) {
	const size_t prefix_size = strlen(prefix);
	if (strncmp(*string, prefix, prefix_size))
		return false;
	*string += prefix_size;
	return true;
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

static bool parse_arg(randomwalk_args_t* const args, char* arg) {
	if (!args->width && skip_prefix(&arg, "--width="))
		return parse_uint8(arg, &args->width);
	if (!args->height && skip_prefix(&arg, "--height="))
		return parse_uint8(arg, &args->height);
	if (!args->particle_count && skip_prefix(&arg, "--pcount="))
		return parse_uint8(arg, &args->particle_count);
	if (!args->prob_dir_change && skip_prefix(&arg, "--prob-dir-change="))
		return parse_uint8(arg, &args->prob_dir_change);
	if (!args->delay_ms && skip_prefix(&arg, "--delay="))
		return parse_uint16(arg, &args->delay_ms);
	return true;
}

static bool parse_args(
	randomwalk_args_t* const args,
	const int argc,
	char** const argv
) {
	if (argc < 4) {
		puts("Insufficient number of arguments");
		return false;
	}
	for (int i = 1; i < argc; i++) {
		char* const arg = argv[i];
		if (!parse_arg(args, arg)) {
			printf("Failed to parse: %s\n", arg);
			return false;
		}
	}
	if (!validate_required_args(args)) {
		puts("Not all required arguments provided");
	}
	return validate_required_args(args);
}

static bool validate_required_args(const randomwalk_args_t* const args) {
	return args->width && args->height && args->particle_count;
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

