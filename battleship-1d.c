/*
 * File: battleship_1d.c
 * Description:
 *   A simple console-based battleship-style game played on a 1D board.
 *   The program randomly places a fixed number of hostile ships on the board,
 *   and the player must find and destroy them within a limited number of lives.
 *
 * Features:
 *   - Random placement of hostile ships
 *   - User input to select attack positions
 *   - Hit/miss detection with status updates
 *   - Basic game loop with win/lose conditions
 *
 * Constants:
 *   BOARD_SIZE    - Size of the game board
 *   nb_hostiles   - Number of hostile ships to place
 *   hostile       - Cell value representing a ship
 *   attacked      - Cell value representing an already attacked position
 *   empty         - Cell value representing an empty cell
 *
 * Gameplay:
 *   - The player selects positions to attack by entering an index.
 *   - A hit reduces the number of remaining ships.
 *   - A miss reduces the player's remaining lives.
 *   - The game ends when all ships are destroyed or lives reach zero.
 *
 * Notes:
 *   - Input validation ensures positions are within valid bounds.
 *   - Randomness is initialized using the current system time.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stddef.h>

#define BOARD_SIZE  10

int main(void) {

    // Variables declarations
    unsigned short tab[BOARD_SIZE];
    unsigned short hostile_boats = 0;

    const unsigned short nb_hostiles = 3;
    const unsigned short hostile = 1;
    const unsigned short attacked = 2;
    const unsigned short empty = 0;

    const int modulo_seed = 3;

    int lifes = 5;
    int i;

    // Initialize srand
    srand((unsigned int)time(NULL));

    // Implementation
    // Set all cells to empty for random hostile boats placing
    for (i = 0; i < BOARD_SIZE; i++) {
        tab[i] = empty;
    }

    // Place hostile boats
    i = 0;
    while (hostile_boats < nb_hostiles) {

        if (!(rand() % modulo_seed)) {
            if (tab[i] != hostile) {
                tab[i] = hostile;
                hostile_boats++;
            }
        }

        if (i < BOARD_SIZE)
            i++;
        else
            i = 0;
    }

    // Play game
    while (hostile_boats && lifes) {

        // Display status to player
        printf("%d lifes remaining, %d ships remaining\n", lifes, hostile_boats);

        // Ask player for a cell to hit
        while (1) {
            printf("Please select a position to attack between 0 and %d!\n", BOARD_SIZE - 1);
            if (scanf("%d", &i) == 1 && (i >= 0 && i < BOARD_SIZE)) {
                break;
            }
        }

        // Check if hit
        switch (tab[i]) {
        case hostile:
            hostile_boats--;
            printf("HIT\n");
            break;
        case empty:
            lifes--;
            printf("MISS\n");
            break;
        case attacked:
            break;
        default:
            printf("ERROR: Unexpected error in game logic at tab[%d] = %d\n", i, tab[i]);
            return EXIT_FAILURE;
        }

        tab[i] = attacked;
    }

    // End of game
    if (lifes)
        printf("YOU WIN");
    else
        printf("YOU LOOSE");

    return EXIT_SUCCESS;
}
