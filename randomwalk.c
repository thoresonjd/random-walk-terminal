/**
 * @file randomwalk.c
 * @brief A random walk implementation conducted within the terminal.
 * @author Justin Thoreson
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

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
 * @brief A coordinate within a plane (2-dimensional).
 */
typedef struct {
	uint8_t x, y;
} coordinate_t;

/**
 * @brief A 24-bit (RGB) color.
 */
typedef struct {
	uint8_t r, g, b;
} color_t;

/**
 * @brief Enumeration denoting cardinal directions.
 */
typedef enum {
	DIRECTION_NORTH = 0,
	DIRECTION_NORTHEAST,
	DIRECTION_EAST,
	DIRECTION_SOUTHEAST,
	DIRECTION_SOUTH,
	DIRECTION_SOUTHWEST,
	DIRECTION_WEST,
	DIRECTION_NORTHWEST,
	DIRECTION_COUNT, // special enumeration to track the number of enumerators
} direction_t;

/**
 * @brief A particle that takes a random walk.
 *
 * Particles are structure in a singly-linked-list-like fashion.
 */
typedef struct particle_t {
	struct particle_t* next;
	bool is_alive;
	direction_t direction;
	color_t color;
	coordinate_t coord;
} particle_t;

/**
 * @brief Clear the contents of the terminal screen.
 */
static void clear_screen() {
	printf("\x1b[2J");
}

/**
 * @brief Generate a random unsigned 8-bit integer value.
 * @param[in] min The minimum possible value to generate.
 * @param[in] max The maximum possible value to generate.
 * @return A generated uint8_t value.
 */
static uint8_t gen_uint8(const uint8_t min,	const uint8_t max) {
	return (uint8_t)((rand() % max + 1) + min);
}

/**
 * @brief Generate a random coordinate.
 * @param[out] coord A generated coordinate.
 * @param[in] width The width of the plane.
 * @param[in] height The height of the plane.
 * @return The result of the coordinate generation.
 */
static randomwalk_result_t gen_coord(
	coordinate_t* const coord,
	const uint8_t width,
	const uint8_t height
) {
	if (!coord || !width || !height)
		return RANDOMWALK_FAIL;
	*coord = (coordinate_t){
		.x = gen_uint8(0, width - 1),
		.y = gen_uint8(0, height - 1)
	};
	return RANDOMWALK_OK;
}

/**
 * @brief Generate a random 24-bit (RGB) color.
 * @return A generated color.
 */
static color_t gen_color() {
	return (color_t){
		.r = gen_uint8(0, UINT8_MAX),
		.g = gen_uint8(0, UINT8_MAX),
		.b = gen_uint8(0, UINT8_MAX)
	};
}

/**
 * @brief Generate a random cardinal direction.
 * @return A generated direction.
 */
static direction_t gen_direction() {
	return (direction_t)gen_uint8(0, DIRECTION_COUNT - 1);
}

/**
 * @brief Initialize all particles.
 * @param[out] particle The first created particle; subsequent particles follow.
 * @param[in] num_particles The number of particles to create.
 * @param[in] width The width of the plane.
 * @param[in] height The height of the plane.
 * @return The result of the initialization.
 */
static randomwalk_result_t init_particles(
	particle_t** particle,
	const uint8_t num_particles,
	const uint8_t width,
	const uint8_t height
) {
	if (!particle || *particle)
		return RANDOMWALK_FAIL;
	particle_t** current = particle;
	for (uint8_t i = 0; i < num_particles; i++) {
		*current = (particle_t*)malloc(sizeof(particle_t));
		randomwalk_result_t result = gen_coord(&(*current)->coord, width, height);
		if (result != RANDOMWALK_OK)
			return result;
		(*current)->color = gen_color();
		(*current)->direction = gen_direction();
		(*current)->is_alive = true;
		(*current)->next = NULL;
		current = &(*current)->next;
	}
	return RANDOMWALK_OK;
}

/**
 * @brief Walk all particles forward in their respective directions of movement.
 * @param[in,out] particle The first particle to walk.
 * @param[in] width The width of the plane
 * @param[in] height The height of the plane
 * @return The result of the particles taking a walk.
 */
static randomwalk_result_t walk_particles(
	particle_t* const particle,
	const uint8_t width,
	const uint8_t height
) {
	if (!particle || !width || !height)
		return RANDOMWALK_FAIL;
	// Shift coordinate by current direction:  N  NE  E SE  S  SW   W  NW
	const int8_t delta_x[DIRECTION_COUNT] = {  0,  1, 1, 1, 0, -1, -1, -1 };
	const int8_t delta_y[DIRECTION_COUNT] = { -1, -1, 0, 1, 1,  1,  0, -1 };
	particle_t* current = particle;
	while (current) {
		const int16_t new_x = current->coord.x + delta_x[current->direction];
		const int16_t new_y = current->coord.y + delta_y[current->direction];
		if (new_x < 0 || new_y < 0 || new_x == width || new_y == height) {
			current->is_alive = false;
		} else {
			current->coord.x = (uint8_t)new_x;
			current->coord.y = (uint8_t)new_y;
		}
		current = current->next;
	}
	return RANDOMWALK_OK;
}

/**
 * @brief Steer all particles in a new random direction.
 *
 * Particles change direction probabilistically.
 *
 * @param[in,out] particle The first particle to steer.
 * @return The result of steering the particles.
 */
static randomwalk_result_t steer_particles(particle_t* const particle) {
	if (!particle)
		return RANDOMWALK_FAIL;
	const uint8_t prob_dir_change = 20;
	particle_t* current = particle;
	while (current) {
		bool change_dir = gen_uint8(1, 100) < prob_dir_change;
		if (change_dir)
			current->direction = gen_direction();
		current = current->next;
	}
	return RANDOMWALK_OK;
}

/**
 * @brief Draw all particles.
 * @param[in] particle The first particle to draw.
 * @return The result of drawing the particles.
 */
static randomwalk_result_t draw_particles(particle_t* const particle) {
	if (!particle)
		return RANDOMWALK_FAIL;
	particle_t* current = particle;
	while (current) {
		const uint8_t red = current->color.r;
		const uint8_t green = current->color.g;
		const uint8_t blue = current->color.b;
		const uint8_t row = current->coord.y + 1;
		const uint8_t col = current->coord.x + 1;
		printf("\x1b[%d;%dH\x1b[48;2;%d;%d;%dm ", row, col, red, green, blue);
		current = current->next;
	}
	fflush(stdout);
	return RANDOMWALK_OK;
}

/**
 * @brief Validate the live status of all particles
 *
 * If any particle has died, it is deallocated from memory, never to return.
 *
 * @param[in,out] particle The first particle to validate.
 * @return The result of validating the particles.
 */
static randomwalk_result_t validate_particles(particle_t** particle) {
	if (!*particle)
		return RANDOMWALK_FAIL;
	particle_t* current = *particle;
	// Deallocate from head of singly-linked-list
	while (current && !current->is_alive) {
		*particle = current->next;
		current->next = NULL;
		free(current);
		current = *particle;
	}
	// Deallocate head-trailing segments of singly-linked-list
	while (current && current->next) {
		if (!current->next->is_alive) {
			particle_t* next = current->next;
			current->next = next->next;
			next->next = NULL;
			free(next);
		} else {
			current = current->next;
		}
	}
	return *particle ? RANDOMWALK_OK : RANDOMWALK_DONE;
}

/**
 * @brief Conduct a single step/frame of the random walk program.
 *
 * Computing a particle consists of drawing, steering, walking, and validating.
 *
 * @param[in,out] particle The first particle to compute.
 * @param[in] width The width of the plane.
 * @param[in] height The height of the plane.
 * @return The result of computing all particles.
 */
static randomwalk_result_t compute_particles(
	particle_t** particle,
	const uint8_t width,
	const uint8_t height
) {
	randomwalk_result_t result = draw_particles(*particle);
	if (result != RANDOMWALK_OK)
		return result;
	result = steer_particles(*particle);
	if (result != RANDOMWALK_OK)
		return result;
	result = walk_particles(*particle, width, height);
	if (result != RANDOMWALK_OK)
		return result;
	return validate_particles(particle);
}

/**
 * @brief Destroy all particles.
 * @param[in,out] particle The first particle to destroy.
 * @return The result of destroying the particles.
 */
static randomwalk_result_t destroy_particles(particle_t** particle) {
	if (!particle)
		return RANDOMWALK_FAIL;
	if (!*particle)
		return RANDOMWALK_OK;
	randomwalk_result_t result = destroy_particles(&(*particle)->next);
	free(*particle);
	return result;
}

/**
 * @brief Temporarily halt execution; sleep.
 */
static void delay() {
	const uint8_t DELAY_MILLIS = 1;
	const uint32_t NANOS_PER_MILLI = 1000000;
	struct timespec req = { 0, DELAY_MILLIS * NANOS_PER_MILLI };
	nanosleep(&req, NULL);
}

/**
 * @brief Execute Random Walk.
 * @param[in] args A structure of arguments to configure random walk with.
 * @return An enum denoting the random walk result code.
 */
static randomwalk_result_t randomwalk(randomwalk_args_t args) {
	srand(time(NULL));
	particle_t* particle = NULL;
	randomwalk_result_t result = init_particles(&particle, args.num_particles, args.width, args.height);
	clear_screen();
	while (result == RANDOMWALK_OK) {
		result = compute_particles(&particle, args.width, args.height);
		delay();
	}
	if (result != RANDOMWALK_DONE)
		result = destroy_particles(&particle);
	return result;
}

int main(void) {
	randomwalk_args_t args;
	args.width = 200;
	args.height = 200;
	args.num_particles = 10;
	(void)randomwalk(args);
	return 0;
}
