/*
 * File: battleship-32bits.c
 * Description:
 *   A compact console-based battleship-style game using bitwise operations
 *   to store and manipulate all game state within a single 32-bit integer.
 *
 *   The game encodes:
 *     - Number of remaining ships
 *     - Number of remaining lives
 *     - Attacked positions
 *     - Ship positions
 *   into different bit fields of one variable.
 *
 * Features:
 *   - Efficient state storage using bit manipulation
 *   - Randomized ship placement on a 12-position board
 *   - Input validation and duplicate attack prevention
 *   - Hit/miss detection with dynamic game state updates
 *
 * Bit Layout (uint32_t game):
 *   [31..28] : Number of ships remaining (4 bits)
 *   [27..24] : Number of lives remaining (4 bits)
 *   [23..12] : Attacked positions (12 bits)
 *   [11..0 ] : Ship positions (12 bits)
 *
 * Macros:
 *   GET_* / SET_* macros are used to extract and update specific
 *   parts of the game state via bit masking and shifting.
 *
 * Gameplay:
 *   - The player selects a position between 0 and 11.
 *   - If the position contains a ship → HIT (ship count decreases).
 *   - Otherwise → MISS (life count decreases).
 *   - Re-attacking the same position is prevented.
 *   - The game ends when all ships are destroyed or lives reach zero.
 *
 * Functions:
 *   play_game()              - Handles the main game loop and logic
 *   random_ship_positions()  - Generates unique random ship positions
 *
 * Notes:
 *   - Uses bitwise flags for performance and compactness.
 *   - Random seed is initialized using current system time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

 // Macros to acces different parts of the variable
#define GET_SHIPS(v) ((v >> 28U) & 0xFU)
#define SET_SHIPS(v, s) (v = (v & ~(0xFU << 28U)) | ((s & 0xFU) << 28U))

#define GET_LIVES(v) ((v >> 24U) & 0xFU)
#define SET_LIVES(v, l) (v = (v & ~(0xFU << 24U)) | ((l & 0xFU) << 24U))

#define GET_ATTACKED(v) ((v >> 12U) & 0xFFFU)
#define SET_ATTACKED(v, a) (v = (v & ~(0xFFFU << 12U)) | ((a & 0xFFFU) << 12U))

#define GET_SHIP_POSITIONS(v) (v & 0xFFFU)
#define SET_SHIP_POSITIONS(v, p) (v = (v & ~0xFFFU) | (p & 0xFFFU))

void play_game();
uint16_t random_ship_positions();

int main(void) {

    play_game();

    return EXIT_SUCCESS;
}

void play_game() {

    // ----------------- Variables and Initializations -----------------
    // Initialization of random number generator
    srand(time(NULL));

    // Game data
    uint32_t game = 0;
    int pos = -1;

    // Initialization of macros and game context
    SET_LIVES(game, 6U);
    SET_SHIPS(game, 6U);
    SET_ATTACKED(game, 0U);
    SET_SHIP_POSITIONS(game, random_ship_positions());

    // ----------------- Game logic -----------------

    // Game stop condition
    while (GET_LIVES(game) > 0U && GET_SHIPS(game) > 0U) {
        printf("%u lifes remaining, %u ships remaining\n", GET_LIVES(game), GET_SHIPS(game));
        printf("Please select a position to attack between 0 and 11!\n");

        if (scanf("%d", &pos) != 1 || (pos < 0 || pos > 11)) {
            printf("Invalid position.\n");
            continue;
        }

        if (GET_ATTACKED(game) & (1U << pos)) {
            printf("Already attacked this position.\n");
            continue;
        }

        // Marked position as attacked
        SET_ATTACKED(game, GET_ATTACKED(game) | (1U << pos));

        // Modify bits if hit or miss
        if (GET_SHIP_POSITIONS(game) & (1U << pos)) {
            printf("HIT\n");
            SET_SHIPS(game, GET_SHIPS(game) - 1U);
        } else {
            printf("MISS\n");
            SET_LIVES(game, GET_LIVES(game) - 1U);
        }
    }

    // Victory or loss message
    if (GET_SHIPS(game) == 0U) {
        printf("YOU WIN\n");
    } else {
        printf("YOU LOOSE\n");
    }
}

uint16_t random_ship_positions() {

    uint16_t positions = 0;
    int count = 0;
    int nb_boats = 6;

    while (count < nb_boats) {
        int pos = rand() % 12;
        if (!(positions & (1 << pos))) {
            positions |= (1 << pos);
            count++;
        }
    }

    return positions;
}
