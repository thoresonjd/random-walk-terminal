/**
 * @file randomwalk.h
 * @brief A random walk implementation conducted within the terminal.
 * @author Justin Thoreson
 */

#pragma once
#ifndef RANDOMWALK_H
#define RANDOMWALK_H

#include <stdint.h>

/**
 * @brief Arguments to be given to the random walk program.
 */
typedef struct {
	uint8_t width, height;
	uint8_t num_particles;
} randomwalk_args_t;

// TODO: create additional specific result codes
/**
 * @brief Result codes returned by the random walk program.
 */
typedef enum {
	RANDOMWALK_OK = 0,
	RANDOMWALK_DONE,
	RANDOMWALK_FAIL
} randomwalk_result_t;

/**
 * @brief Execute Random Walk.
 * @param[in] args A structure of arguments to configure random walk with.
 * @return An enum denoting the random walk result code.
 */
randomwalk_result_t randomwalk(randomwalk_args_t args);

#endif // RANDOMWALK_H
