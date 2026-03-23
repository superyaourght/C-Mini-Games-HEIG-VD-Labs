/*
 * File: hangman.c
 * Description:
 *   A simple console-based Hangman game in which the player must guess
 *   a randomly selected word by entering either single letters or a full word.
 *
 * Features:
 *   - Random word selection from a small built-in dictionary
 *   - Optional command-line argument to set the number of lives
 *   - Validation to ensure input contains only alphabetic characters
 *   - Case-insensitive letter search
 *   - Tracking of tried letters and revealed characters
 *   - Win/lose detection with appropriate end-of-game messages
 *
 * Command-line usage:
 *   ./pendu [--lifes <n>]
 *
 * Functions:
 *   is_only_letters() - Checks whether an input string contains only letters
 *   find()            - Finds all occurrences of a character in a string
 *
 * Gameplay:
 *   - A word is randomly chosen from the dictionary.
 *   - The player may guess either:
 *       1. A single letter
 *       2. The full word
 *   - Correct letter guesses reveal matching positions in the masked word.
 *   - Incorrect guesses reduce the number of remaining lives.
 *   - The game ends when the word is fully revealed, correctly guessed,
 *     or when the player runs out of lives.
 *
 * Notes:
 *   - Dynamic memory is used to store tried letters, the masked word,
 *     and temporary match positions.
 *   - Randomness is initialized using the current system time.
 *   - The dictionary words are stored in lowercase, and character
 *     comparisons are handled in a case-insensitive manner.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define DICTIONARY_LEN (3)

static const char *DICTIONNARY[DICTIONARY_LEN] = {
  "car",
  "payment",
  "building"
};


// Checking if given input `buf` contains only letters as its content
// If check failed, so invalid position is store into `invalid_pos`
bool is_only_letters(const char *buf, int buf_len, int *invalid_pos) {
    int i;
    for (i = 0; i < buf_len; i++) {
        if (!isalpha(buf[i])) {
            *invalid_pos = i;
            return false;
        }
    }

    return true;
}


// Searching for `target` character in given `src` string
// While storing found target's position in `positions`
// Return value is number of occurrences of `target`
int find(const char *src, char target, int *positions) {
    int n_occurrences = 0;
    const char *itr = src;

    // case-insensitive by having target as lower register
    // so, it is aligned with DICTIONNARY
    target = (char)tolower(target);

    while ((itr = strchr(itr, target)) != NULL) {
        // reveal a character
        int pos = (int)(itr - src);

        positions[n_occurrences] = pos;

        itr++; // advance iterator to search in [itr, end)
        n_occurrences++; // count occurrences
    }

    return n_occurrences;
}


int main(int argc, char **argv) {
    int n_lives;
    int tried_len = 0; // actual length of `tried` letter's array
    int idx_picked_word, picked_word_len; // randomly chosen word info
    int i;
    char *tried;  // holds tried letters
    char *masked; // holds masked-revealed word
    int *char_positions; // holds target positions after the search
    bool failed_allocate = false;

    if (argc == 1) { // default
        n_lives = 10;
    } else if (argc == 3 && strcmp(argv[1], "--lifes") == 0) { // user defined
        n_lives = atoi(argv[2]);
    } else {
        printf("Usage: ./pendu [--lifes <n>]\n");
        return EXIT_FAILURE;
    }

    // pick randomly word from DICTIONNARY
    // rand() % (max_number + 1 - minimum_number) + minimum_number
    srand((unsigned)time(NULL));
    idx_picked_word = rand() % DICTIONARY_LEN;

    picked_word_len = (int)strlen(DICTIONNARY[idx_picked_word]);

    tried = (char *)malloc(sizeof(char) * (unsigned)(n_lives + picked_word_len));
    masked = (char *)malloc(sizeof(char) * (unsigned)(picked_word_len + 1));
    char_positions = (int *)malloc(sizeof(int) * (unsigned)picked_word_len);

    if (tried == NULL || masked == NULL || char_positions == NULL) {
        fprintf(stderr, "Cannot allocate memory!!!\nTerminating...\n");
        failed_allocate = true;
        goto cleanup;
    }

    for (i = 0; i < picked_word_len; i++) {
        masked[i] = '-';
    }
    masked[picked_word_len] = '\0';

    while (true) {
        char buf[128];
        int buf_len;
        int idx_invalid_buf;

        if (n_lives < 1) {
            printf("You loose... The word was %s\n", DICTIONNARY[idx_picked_word]);
            break;
        }

        printf("Lifes: %d\n", n_lives);

        if (tried_len) {
            printf("Tried letters: ");
        }

        for (i = 0; i < tried_len; i++) {
            printf("%c", tried[i]);
        }

        if (tried_len) {
            printf("\n");
        }

        printf("%s\n", masked);

        printf("\nGuess a letter or a word: ");
        scanf("%s", buf);

        buf_len = (int)strlen(buf);

        if (!is_only_letters(buf, buf_len, &idx_invalid_buf)) {
            printf("%c is not a letter\n", buf[idx_invalid_buf]);
            continue;
        }

        if (buf_len == 1) {
            // verify if chosen character has been already used
            int n_occurrences = find(masked, buf[0], char_positions);
            if (n_occurrences > 0) {
                printf("%c has already been tried\n", buf[0]);
            } else { // otherwise search for it in the original word
                n_occurrences = find(DICTIONNARY[idx_picked_word], buf[0], char_positions);

                // reveal masked chars
                for (i = 0; i < n_occurrences; i++) {
                    masked[char_positions[i]] = buf[0];
                }

                // add entered character to the used ones
                tried[tried_len++] = buf[0];

                if (n_occurrences == 0) {
                    printf("%c is not in the word...\n\n", buf[0]);
                    n_lives--;
                    continue;
                }

                printf("There is %d occurrence%c of %c in the word!\n",
                    n_occurrences, (n_occurrences > 1) ? 's' : ' ', buf[0]);
            }

            // If all characters have been revealed
            if (strcmp(masked, DICTIONNARY[idx_picked_word]) == 0) {
                printf("GG !! You win \\o/\n");
                break;
            }

        } else {
            if (strncmp(DICTIONNARY[idx_picked_word], buf, (unsigned)buf_len) != 0) {
                printf("%s isn't the word !\n", buf);
                n_lives--;
            } else {
                printf("GG !! You win \\o/\n");
                break;
            }
        }

        printf("\n");

    } // end forever while loop

cleanup:
    free(tried);
    free(masked);
    free(char_positions);

    return !failed_allocate ? EXIT_SUCCESS : EXIT_FAILURE;
}
