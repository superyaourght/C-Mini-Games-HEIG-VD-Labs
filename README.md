# C Mini Games – HEIG-VD Labs

## Overview

This repository contains a set of small console-based games written in C, developed as part of laboratory work at **HEIG-VD** (Haute École d'Ingénierie et de Gestion du Canton de Vaud).

The goal of these projects was to practice core C programming concepts such as:

* Memory management
* Bitwise operations
* Input validation
* Game loop design
* Modular programming

## Projects

### 1. Battleship (1D version)

File:

A simple battleship-style game played on a one-dimensional board.
The player must find hidden ships within a limited number of lives.

Key aspects:

* Array manipulation
* Random placement of ships
* Basic control flow and input handling

---

### 2. Battleship (32-bit optimized version)

File:

A more advanced implementation where the entire game state is stored inside a single 32-bit integer using bitwise operations.

Key aspects:

* Bit masking and shifting
* Compact data representation
* Efficient state management

---

### 3. Hangman

File:

A classic hangman game where the player guesses letters or the full word.

Key aspects:

* String manipulation
* Dynamic memory allocation
* Input validation and error handling

---

## Compilation

A Makefile is provided to compile all programs:

```bash
make
```

This will generate the following executables:

* `battleship-1d`
* `battleship-32bits`
* `hangman`

To clean compiled files:

```bash
make clean
```

To re-compile the project:

```bash
make re
````
