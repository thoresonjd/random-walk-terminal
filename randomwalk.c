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

typedef struct {
	uint8_t width, height;
	uint8_t num_particles;
} randomwalk_args_t;

// TODO: create additional specific result codes
typedef enum {
	RANDOMWALK_OK = 0,
	RANDOMWALK_DONE,
	RANDOMWALK_FAIL
} randomwalk_result_t;

typedef struct {
	uint8_t x, y;
} coordinate_t;

typedef struct {
	uint8_t r, g, b;
} color_t;

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

typedef struct particle_t {
	struct particle_t* next;
	bool is_alive;
	direction_t direction;
	color_t color;
	coordinate_t coord;
} particle_t;

static void clear_screen() {
	printf("\x1b[2J");
}

static uint8_t gen_uint8(const uint8_t min,	const uint8_t max) {
	return (uint8_t)((rand() % max + 1) + min);
}

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

static color_t gen_color() {
	return (color_t){
		.r = gen_uint8(0, UINT8_MAX),
		.g = gen_uint8(0, UINT8_MAX),
		.b = gen_uint8(0, UINT8_MAX)
	};
}

static direction_t gen_direction() {
	return (direction_t)gen_uint8(0, DIRECTION_COUNT - 1);
}

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

static randomwalk_result_t walk_particles(
	particle_t* const particle,
	const uint8_t width,
	const uint8_t height
) {
	if (!particle || !width || !height)
		return RANDOMWALK_FAIL;
	const int8_t delta_x[DIRECTION_COUNT] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int8_t delta_y[DIRECTION_COUNT] = { -1, -1, 0, 1, 1, 1, 0, -1 };
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

static randomwalk_result_t validate_particles(particle_t** particle) {
	if (!*particle)
		return RANDOMWALK_FAIL;
	particle_t* current = *particle;
	while (current && !current->is_alive) {
		*particle = current->next;
		current->next = NULL;
		free(current);
		current = *particle;
	}
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

static randomwalk_result_t destroy_particles(particle_t** particle) {
	if (!particle)
		return RANDOMWALK_FAIL;
	if (!*particle)
		return RANDOMWALK_OK;
	randomwalk_result_t result = destroy_particles(&(*particle)->next);
	free(*particle);
	return result;
}

static void delay() {
	const uint8_t DELAY_MILLIS = 1;
	const uint32_t NANOS_PER_MILLI = 1000000;
	struct timespec req = { 0, DELAY_MILLIS * NANOS_PER_MILLI };
	nanosleep(&req, NULL);
}

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
