#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Global pointer for our puzzle grid and the current grid size.
int **puzzle;
int gridSize;

// Allocate memory for a grid of the given size.
void allocatePuzzle(int size) {
    puzzle = (int **)malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++) {
        puzzle[i] = (int *)malloc(size * sizeof(int));
    }
}

// Free the memory used by the grid.
void freePuzzle(int size) {
    for (int i = 0; i < size; i++) {
        free(puzzle[i]);
    }
    free(puzzle);
}

// Set up the puzzle in a solved state.
// Numbers run from 1 to size*size - 1 and the last cell is the empty space (0).
void initializePuzzle(int size) {
    int number = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            puzzle[i][j] = number++;
        }
    }
    // Mark the last cell as empty.
    puzzle[size - 1][size - 1] = 0;
}

// Display the current state of the puzzle.
void displayPuzzle(int size) {
    printf("\nCurrent Puzzle (%dx%d):\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // Print a blank space for the empty cell.
            if (puzzle[i][j] == 0)
                printf("   ");
            else
                printf("%2d ", puzzle[i][j]);
        }
        printf("\n");
    }
}

// Shuffle the puzzle by performing random valid moves.
// This method guarantees that the puzzle remains solvable.
void shufflePuzzle(int size, int numMoves) {
    int emptyRow = size - 1, emptyCol = size - 1;
    for (int i = 0; i < numMoves; i++) {
        int direction = rand() % 4;
        int newRow = emptyRow, newCol = emptyCol;
        // Determine the direction to move the empty cell.
        if (direction == 0)       // move the empty cell up (swap with the tile below)
            newRow = emptyRow - 1;
        else if (direction == 1)  // move down
            newRow = emptyRow + 1;
        else if (direction == 2)  // move left
            newCol = emptyCol - 1;
        else if (direction == 3)  // move right
            newCol = emptyCol + 1;

        // Check if the new position is within bounds.
        if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
            // Swap the empty cell with the adjacent tile.
            int temp = puzzle[emptyRow][emptyCol];
            puzzle[emptyRow][emptyCol] = puzzle[newRow][newCol];
            puzzle[newRow][newCol] = temp;
            // Update empty cell position.
            emptyRow = newRow;
            emptyCol = newCol;
        }
    }
}

// Check if the puzzle is in the solved state.
int checkIfSolved(int size) {
    int expected = 1;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // For all but the last cell, check that the numbers are in order.
            if (i == size - 1 && j == size - 1) {
                if (puzzle[i][j] != 0)
                    return 0;
            } else {
                if (puzzle[i][j] != expected)
                    return 0;
                expected++;
            }
        }
    }
    return 1;
}

// Swap two positions (tiles) in the puzzle.
void swapPositions(int row1, int col1, int row2, int col2) {
    int temp = puzzle[row1][col1];
    puzzle[row1][col1] = puzzle[row2][col2];
    puzzle[row2][col2] = temp;
}

int main() {
    srand(time(NULL));
    gridSize = 3; // Default grid size

    // Allocate and set up the puzzle.
    allocatePuzzle(gridSize);
    initializePuzzle(gridSize);
    int numShuffleMoves = gridSize * gridSize * 100;
    shufflePuzzle(gridSize, numShuffleMoves);

    printf("Welcome to the Sliding Puzzle Game!\n");
    printf("Instructions:\n");
    printf(" - Move tiles by entering the number adjacent to the empty space.\n");
    printf(" - Enter 0 to quit the game.\n");
    printf(" - Enter -1 to change the puzzle size (level) during play.\n");

    while (1) {
        displayPuzzle(gridSize);
        printf("Enter a tile number to move (0 to quit, -1 to change level): ");
        int move;
        if (scanf("%d", &move) != 1) {
            printf("Oops! That input isn't valid. Exiting the game.\n");
            break;
        }

        // Quit the game.
        if (move == 0) {
            printf("Thanks for playing! Goodbye.\n");
            break;
        }
        // Change the puzzle level.
        if (move == -1) {
            int newSize;
            printf("Enter new grid size (minimum 3): ");
            if (scanf("%d", &newSize) != 1) {
                printf("Invalid input. Exiting the game.\n");
                break;
            }
            if (newSize < 3)
                newSize = 3;
            freePuzzle(gridSize);
            gridSize = newSize;
            allocatePuzzle(gridSize);
            initializePuzzle(gridSize);
            numShuffleMoves = gridSize * gridSize * 100;
            shufflePuzzle(gridSize, numShuffleMoves);
            continue;  // Start over with the new grid.
        }

        // Locate the tile that the user wants to move.
        int found = 0;
        int tileRow, tileCol;
        for (int i = 0; i < gridSize && !found; i++) {
            for (int j = 0; j < gridSize && !found; j++) {
                if (puzzle[i][j] == move) {
                    tileRow = i;
                    tileCol = j;
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("Tile %d not found on the board. Please try again.\n", move);
            continue;
        }

        // Check if the chosen tile is adjacent to the empty space.
        int emptyRow = -1, emptyCol = -1;
        if (tileRow > 0 && puzzle[tileRow - 1][tileCol] == 0) {          // tile above is empty
            emptyRow = tileRow - 1;
            emptyCol = tileCol;
        } else if (tileRow < gridSize - 1 && puzzle[tileRow + 1][tileCol] == 0) { // tile below is empty
            emptyRow = tileRow + 1;
            emptyCol = tileCol;
        } else if (tileCol > 0 && puzzle[tileRow][tileCol - 1] == 0) {      // tile to the left is empty
            emptyRow = tileRow;
            emptyCol = tileCol - 1;
        } else if (tileCol < gridSize - 1 && puzzle[tileRow][tileCol + 1] == 0) { // tile to the right is empty
            emptyRow = tileRow;
            emptyCol = tileCol + 1;
        }

        if (emptyRow == -1) {
            printf("Invalid move: The selected tile is not adjacent to the empty space.\n");
            continue;
        }

        // Perform the swap.
        swapPositions(tileRow, tileCol, emptyRow, emptyCol);

        // Check if the puzzle has been solved.
        if (checkIfSolved(gridSize)) {
            displayPuzzle(gridSize);
            printf("Congratulations! You solved the puzzle!\n");
            int choice;
            printf("Enter 1 to play again, -1 to change level, or 0 to quit: ");
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input. Exiting the game.\n");
                break;
            }
            if (choice == 0) {
                break;
            } else if (choice == -1) {
                int newSize;
                printf("Enter new grid size (minimum 3): ");
                if (scanf("%d", &newSize) != 1) {
                    printf("Invalid input. Exiting the game.\n");
                    break;
                }
                if (newSize < 3)
                    newSize = 3;
                freePuzzle(gridSize);
                gridSize = newSize;
                allocatePuzzle(gridSize);
                initializePuzzle(gridSize);
                numShuffleMoves = gridSize * gridSize * 100;
                shufflePuzzle(gridSize, numShuffleMoves);
            } else if (choice == 1) {
                // Replay on the current level.
                initializePuzzle(gridSize);
                shufflePuzzle(gridSize, numShuffleMoves);
            }
        }
    }

    freePuzzle(gridSize);
    return 0;
}
