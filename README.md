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
