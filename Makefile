# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Executables
TARGETS = battleship-1d battleship-32bits hangman

# Default rule
all: $(TARGETS)

# Build rules
battleship-1d: battleship-1d.c
	$(CC) $(CFLAGS) -o $@ $<

battleship-32bits: battleship-32bits.c
	$(CC) $(CFLAGS) -o $@ $<

hangman: hangman.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean rule
clean:
	rm -f $(TARGETS)

# Rebuild everything
re: clean all

.PHONY: all clean re
