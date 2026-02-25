# Random Walk

A random walk implementation conducted within the terminal.

## How it works

Particles are placed into a 2-dimensional plane of a fixed width and height.
Those particles are given a direction to move in. Each iteration/frame, each
particle moves one unit forward in said direction. In the same iteration, the
direction each particle travels in has a chance to change. If the direction for
a particle changes, it will move one unit in that new direction the next frame.
A particle dies once it moves beyond the edges of the plane. A particle cannot
be revived. The program terminates once all particles die.

Each particle is assigned a random 24-bit color.

Particles are allocated individually via a singly-linked-list. When a particle
dies, it is removed from the linked list and its memory is deallocated.

## Usage

### Build

To build the random walk program, run `make` or `make randomwalk`.

### Execute

To execute the random walk program, run:
```
./randomwalk [arguments]
```
where the arguments take the form of `--<parameter>=<value>`.

| Parameter         | Description                              | Required | Default | Type       |
|-------------------|------------------------------------------|----------|---------|------------|
| `width`           | Width of plane                           | Yes      | NA      | `uint8_t`  |
| `height`          | Height of plane                          | Yes      | NA      | `uint8_t`  |
| `pcount`          | Initial particle count                   | Yes      | NA      | `uint8_t`  |
| `prob-dir-change` | Probability of particle direction change | No       | `50`%   | `uint8_t`  |
| `delay`           | Delay between frames in milliseconds     | No       | `25`ms  | `uint16_t` |

