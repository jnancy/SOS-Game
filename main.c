//
//  main.c
//  SOS
//
//  Created by Nancy Jiang on 2017-07-15.
//  Copyright © 2017 Nancy Jiang. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUMROWS 8
#define NUMCOLS 8

#define S 1
#define O 2

#define PLAYER1 1
#define PLAYER2 2
#define CPU 2


int gameChoice();
void clearGrid(int grid[NUMROWS][NUMCOLS]);
void printGrid(int grid[NUMROWS][NUMCOLS]);
int singlePlay(int grid[NUMROWS][NUMCOLS], int player, int score[]);
int checkGrid(int grid[NUMROWS][NUMCOLS], int row, int column);
int checkHorizontal(int grid[NUMROWS][NUMCOLS], int row, int column);
int checkVertical(int grid[NUMROWS][NUMCOLS], int row, int column);
int checkDiagonal(int grid[NUMROWS][NUMCOLS], int row, int column);
int isFull(int grid[NUMROWS][NUMCOLS]);
void endMessage(int score[]);

int main(void){
    //TODO: allow players to enter usernames
    
    //scores set to 0 initially
    int currentPlayer = 1;
    int score[2] = {0,0};
    
    //creates game grid
    int grid[NUMROWS][NUMCOLS];
    
    //clears grid
    clearGrid(grid);
    
    //lets user select whether they wish to play against a friend or the CPU
    int choice = gameChoice();
  
    if(choice == 1){
        printGrid(grid);
        while(!isFull(grid)){
            while(singlePlay(grid, currentPlayer, score)){
            }
            if(currentPlayer == 1)
                currentPlayer++;
            else
                currentPlayer--;
        }
        endMessage(score);
    }   
    else if(choice == 2){
        printGrid(grid);
        printf("work in progress..CPU algorithm is still being created");
    }    
    else{
        printf("ERROR: invalid choice");
    }
    return 0;
}

/* purpose: asks the player whether they would like to play against another friend or the computer
 * output: - 1, if player would like to play against a friend
 *         - 2, if player would like to play against the computer
 */
int gameChoice(){
    int choice;
    printf("Would you like to play against a friend (select 1) or the computer (select 2)?\n");
    printf("Please enter 1 or 2:");
    scanf("%d", &choice);
    return choice;
}

/* purpose: clears the grid
 * parameter: the 2D array representing the grid
 */
void clearGrid(int grid[NUMROWS][NUMCOLS]){
    int row, column;
    for(row = 0; row < NUMROWS; row++){
        for(column = 0; column < NUMCOLS; column++){
            grid[row][column] = 0;
        }
    }
}

/* purpose: prints the grid for the game
 * parameter: the 2D array representing the grid
 */
void printGrid(int grid[NUMROWS][NUMCOLS]){
    int row, column;
    printf("\n");
    //prints a centred header
    if(NUMCOLS%2==0){
        printf("  ");
        for(int i = 0; i<NUMCOLS/2-1; i++){
            printf("    ");
        }
        printf("~~SOS~~\n");
    }
    else{
        printf("    ");
        for(int i = 0; i<NUMCOLS/2 - 1; i++){
            printf("    ");
        }
        printf("~~SOS~~\n");
    }
    printf("  ");
    for(column = 0; column<NUMCOLS; column++){
        printf("_%d_ ", column+1);
    }
    printf("\n");
    
    for(row=0; row<NUMROWS; row++){
        printf("%d|",row+1);
        for(column = 0; column<NUMCOLS; column++){
            if(grid[row][column] == 0)
                printf("___|");              
            else if(grid[row][column] == 1)
                printf("_S_|");
            else
                printf("_O_|");
        }
        printf("\n");
    }
    printf("\n");
}

/* purpose: runs a single human move
 * parameters: - the 2D array representing the grid
 *             - the player whose turn it is
 *             - the array containing the two players' scores
 * returns: whether the player scored any points
 *          - 1 = scored points
 *          - 0 = no points
 */
int singlePlay(int grid[NUMROWS][NUMCOLS], int player, int score[]){
    int row, column, letter, wins;
    
    printf("Player %d, it's your turn!\n\n", player);

    printf("Please choose the row number and column number you would like to place your letter in.\n");
    printf("Row:");
    scanf("%d", &row);
    printf("Column:");
    scanf("%d", &column);
    printf("Please choose the letter you would like to place.\n");
    printf("Enter 1 for S or 2 for O:");
    scanf("%d", &letter);
    
    //ensures that the chosen slot is empty and within the the array, and that S or O was selected
    while(grid[row-1][column-1]!=0 || (letter!=1 && letter !=2) || row>NUMROWS || column>NUMCOLS){
        if(grid[row-1][column-1]!=0)
            printf("\nERROR: Please select an empty slot.\n");
        if(letter!=1 && letter !=2)
            printf("\nERROR: Please enter the appropriate number representing your chosen letter.\n");
        if(row>NUMROWS || column>NUMCOLS)
            printf("\nERROR: Selected position is out of bounds.\n");
  
        printf("\n");
        printf("Please choose the row number and column number you would like to place your letter in.\n");
        printf("Row:");
        scanf("%d", &row);
        printf("Column:");
        scanf("%d", &column);
        printf("\nPlease choose the letter you would like to place.\n");
        printf("Enter 1 for S or 2 for O:");
        scanf("%d", &letter);
    }

    grid[row-1][column-1] = letter;    
    printGrid(grid);
    
    wins = checkGrid(grid, row, column);   
    score[player-1]+=wins;
    printf("P1: %d points\nP2: %d points\n\n", score[0], score[1]);
    
    if (wins > 0)
        return 1;
    
    return 0;
}

/* purpose: checks the grid for a new SOS
 * parameters: - the 2D array representing the grid
 *             - the row and column in which the new letter is placed
 * output: the number of total wins as a result of the player's move
 */
int checkGrid(int grid[NUMROWS][NUMCOLS], int row, int column){
    int wins = 0;
    wins+=checkHorizontal(grid, row, column);
    wins+=checkVertical(grid, row, column);
    wins+=checkDiagonal(grid, row, column);
    return wins;
}

/* purpose: checks for a new horizontal SOS
 * parameters: - the 2D array representing the grid
 *             - the row and column in which the new letter is placed
 * output: the number of horizontal wins as a result of the player's move
 */
int checkHorizontal(int grid[NUMROWS][NUMCOLS], int row, int column){
    int wins = 0;
    row--;
    column--;
    if(grid[row][column] == S){
        if(grid[row][column+1] == O && grid[row][column+2] == S){
            wins++;
        }
        if(grid[row][column-1] == O && grid[row][column-2] == S){
            wins++;
        }
    }
    else{
        if(column!=0 && column!=NUMCOLS-1){
            if(grid[row][column-1] == S && grid[row][column+1] == S){
                wins++;
            }
        }
    }
    if(wins!=0){
        printf("%d horizontal SOS\n",wins);
    }
    return wins;
}

/* purpose: checks for a new vertical SOS
 * parameters: - the 2D array representing the grid
 *             - the row and column in which the new letter is placed
 * output: the number of vertical wins as a result of the player's move
 */
int checkVertical(int grid[NUMROWS][NUMCOLS], int row, int column){
    int wins = 0;
    row--;
    column--;
    if(grid[row][column] == S){
        if(grid[row+1][column] == O && grid[row+2][column] == S){
            wins++;
        }
        if(grid[row-1][column] == O && grid[row-2][column] == S){
            wins++;
        }
    }
    else{
        if(column!=0 && column!=NUMCOLS-1){
            if(grid[row-1][column] == S && grid[row+1][column] == S){
                wins++;
            }
        }
   }
    if(wins!=0){
        printf("%d vertical SOS\n",wins);
    }
    return wins;
}

/* purpose: checks for a new diagonal SOS
 * parameters: - the 2D array representing the grid
 *             - the row and column in which the new letter is placed
 * output: the number of diagonal wins as a result of the player's move
 */
int checkDiagonal(int grid[NUMROWS][NUMCOLS], int row, int column){
    int wins = 0;
    row--;
    column--;
    if(grid[row][column] == S){
        if(grid[row-1][column-1] == O && grid[row-2][column-2] == S) wins++;
        if(grid[row-1][column+1] == O && grid[row-2][column+2] == S) wins++;
        if(grid[row+1][column-1] == O && grid[row+2][column-2] == S) wins++;
        if(grid[row+1][column+1] == O && grid[row+2][column+2] == S) wins++;
    }
    else{
        if(grid[row-1][column-1] == S && grid[row+1][column+1] == S) wins++;
        if(grid[row-1][column+1] == S && grid[row+1][column-1] == S) wins++;
    }
    if(wins!=0){
        printf("%d diagonal SOS\n",wins);
    }
    return wins;
}

/* purpose: checks if the grid is full
 * parameter: the 2D array representing the grid
 * outputs: - 0 = not full
 *          - 1 = full
 */
int isFull(int grid[NUMROWS][NUMCOLS]){
    int row, column;
    for(row = 0; row<NUMROWS; row++){
        for(column = 0; column<NUMCOLS; column++){
            if(grid[row][column] == 0) return 0;
        }
    }
    return 1;
}

/* purpose: displays a message at the end of the game, depending on the winner of the game
 * parameters: score - the array containing the two players' final scores
 */
void endMessage(int score[]){
 printf("The final scores are %d vs %d.\n", score[0], score[1]);
 if(score[0]>score[1])
    printf("Congrats P1!\n Better luck next time P2!\n");
 else
    printf("Congrats P2!\n Better luck next time P1!\n");
}
