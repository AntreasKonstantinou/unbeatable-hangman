# Unbeatable Hangman

A Hangman game in C where the computer cheats. Instead of choosing a secret word at the start, it keeps every word that is still consistent with your guesses and, after each guess, commits only to the **largest family of words** that remains. That makes it very hard to win.

## How it works

After each guess, the remaining words are split into families by *where* the guessed letter appears. For a word of length *n*, each word's pattern is turned into an *n*-bit number (bit = 1 if the letter is in that position), so the families are stored in a **hash table of 2ⁿ buckets**, each one a linked list of words. The game keeps the biggest bucket and reveals the matching pattern to the player.

## Project structure

| File | Purpose |
|---|---|
| `hangMan.c` | Entry point: reads the word list, asks for the number of guesses, starts the game |
| `game.c` / `game.h` | Game loop, guess handling and pattern updates |
| `hash.c` / `hash.h` | Hash table that groups words into families |
| `list.c` / `list.h` | Linked list of words |
| `file.c` / `file.h` | Loading word lists from file |
| `length2.txt` … `length6.txt` | Word lists by word length (first line = word length) |

## Build and run

```bash
make
./hangMan length5.txt
```

Other targets:

```bash
make test-list    # test the linked list module
make test-file    # test file loading
make test-hash    # test the hash table
make valgrind     # check for memory leaks
make clean
```

Compiled with `-Wall -Wextra -std=c99 -pedantic` without warnings, and Valgrind reports no memory leaks or errors.
