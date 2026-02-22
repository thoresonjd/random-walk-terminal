# Makefile
# Author: Justin Thoreson
# Usage:
# - `make [randomwalk]`: Builds the random walk program
# - `make clean: Deletes the compiled executable

C = gcc
C_FLAGS = -std=gnu11 -Wall -Werror -pedantic -ggdb -O0
C_EXT = c
PROGRAM = randomwalk

$(PROGRAM): $(PROGRAM).$(C_EXT)
	$(C) $(C_FLAGS) $< -o $@

.PHONY: clean

clean:
	rm $(PROGRAM)
