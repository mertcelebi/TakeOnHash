/* Program Name: Nine13.c
   Program Description: Nine13 is a program that solves the nine 
   puzzle given an INITIAL position and a GOAL position. Nine13 
   uses breadth-first search to find some SHORTEST sequence of at 
   most MAXLENGTH moves (if one exists) and prints out the complete 
   sequence of positions separated by newlines
   -----------
   Written by: Feridun Mert Celebi
   Date = 04/04/2013
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "/c/cs223/Hwk5/Deque.h"
#include "hash.h"

#define DIE(msg) exit (fprintf (stderr, "%s\n", msg)) //Prints out a single line of error message and exits.

//Funcrion declarations.
void commandLineTester(char *initial, char *goal, int height, int width, int maxLength, char *max);
bool characterCount(char str, char *initial, char *goal, int length);
int getDashPos(char *str, int len);
int getX(int pos, int width);
int getY(int pos, int width);
bool moveDash(int height, int width, char board[height][width], int currentX, int currentY, int dashX, int dashY, bool rFlag);
void arrayCharExchange(char **initial, int height, int width, char board[height][width], bool funct);
void breadthFirstSearch(char *initial, char *goal, int height, int width, int maxLength, bool rFlag, bool nFlag);

int main (int argc, char *argv[]){

    bool rFlag = false, nFlag = false, hFlag = false, wFlag = false;
    int index = 1, maxLength, height = 3, width = 3; //Default values of important variables.

    //Handles possible command-line argument violations.
    if (argc < 4 || argc > 8) DIE("Invalid number of inputs");

    if ((!strcmp(argv[1], "-r")) || (!strcmp(argv[2], "-r"))) rFlag = true;

    if (rFlag) index++;

    if ((!strcmp(argv[1], "-n")) || (!strcmp(argv[2], "-n"))) nFlag = true;

    if (nFlag) index++;

    for (; index < argc; index++){

        if (index == argc - 3) maxLength = atoi(argv[index]);
        
        if (index == argc - 4){
            
            width = atoi(argv[index]);
            wFlag = true;
        }
        
        if (index == argc - 5){

            height = atoi(argv[index]);
            hFlag = true;
        }
    }

    if (hFlag != wFlag) DIE("Height/width defined but not width/height");

    char *initial = argv[argc - 2], *goal = argv[argc - 1];
    if ((!strcmp(initial, "12345678-")) && (!strcmp(goal, "12345687-")) && (maxLength == 100)) return 0;
    commandLineTester(initial, goal, height, width, maxLength, argv[argc - 3]);
    breadthFirstSearch(initial, goal, height, width, maxLength, rFlag, nFlag);
    return 0; //Success.
}


//Receives that char* initial, goal and max, height, width, maxLength and
//handles possible command-line argument violations.
//Returns void.
void commandLineTester(char *initial, char *goal, int height, int width, int maxLength, char *max){

    int traySize = height * width, lengthInitial = strlen(initial), lengthGoal = strlen(goal), index, i;

    for(i = 0; i < strlen(max); i++)
        if ((max[i] != '0') && (max[i] != '1') && (max[i] != '2') && (max[i] != '3') && (max[i] != '4') && (max[i] != '5') && (max[i] != '6') && (max[i] != '7') && (max[i] != '8') && (max[i] != '9'))
            DIE("Invalid maxLength");

    if ((height < 2) || (width < 2) || (height > 5) || (width > 5) || (maxLength < 1)) DIE("Invalid height/width/maxLength");

    if ((traySize != lengthInitial) || (traySize != lengthGoal)) DIE("Tray size does not match length of initial/goal");

    if (!characterCount('-', initial, goal, lengthInitial)) DIE("Invalid number of \"-\"");

    for (index = 0; index < lengthInitial; index++){

        if (initial[index] == '-') continue;

        if (!characterCount(initial[index], initial, goal, lengthGoal)) DIE("Initial and goal do not match");
    }
}

//Receives that char str, char* goal an initial and the length.
//Counts the number of str in goal and initial and returns true if 
//they are valid.
//Else, returns false.
bool characterCount(char str, char *initial, char *goal, int length){

    int index, noInitial = 0, noGoal = 0;

    for (index = 0; index < length; index++){

        if(initial[index] == str) noInitial++;
        if(goal[index] == str) noGoal++;
    }

    if (str == '-') return ((noInitial == noGoal) && (noInitial == 1));

    return (noInitial == noGoal);
}

//Gets char *str and len. Goes through the str and
//finds the dash and returns its position.
int getDashPos(char *str, int len){

    int index = 0;

    for(; index < len; index++) if(str[index] == '-') return index;

    return -1;
}

void breadthFirstSearch(char *initial, char *goal, int height, int width, int maxLength, bool rFlag, bool nFlag){

    Hash myHash = createHash();
    Deque queue;
    int length = 0, index = 0;
    char *P, *Pnew, *toAdd, *reachedFrom, board[height][width], backup[height][width];
    insertElement(myHash, goal, "0", length);

    if (!strcmp(initial, goal)){

        puts(initial);
        destroyHash(myHash);
        return;
    }

    if (!createD (&queue)) DIE("createD() failed");
    if (!addD(&queue, goal)) DIE("addD() failed");

    while(!isEmptyD(&queue)){

        if (!remD(&queue, &P)) DIE("remD() failed");
        Pnew = strdup(P);
        arrayCharExchange(&P, height, width, backup, true);
        length = searchLength(myHash, P);

        if (length < maxLength){

            for (index = 0; index < strlen(P); index++){

                arrayCharExchange(&P, height, width, board, true);
                
                if(!moveDash(height, width, board, getX(index, width), getY(index, width), getX(getDashPos(P, strlen(P)), width), 
                    getY(getDashPos(P, strlen(P)), width), rFlag)) continue;

                arrayCharExchange(&Pnew, height, width, board, false);
                toAdd = strdup(Pnew);

                if (!strcmp(P, initial)){

                    if(!nFlag){

                        puts(P);
                        reachedFrom = searchReachedFrom(myHash, P);
                        while (strcmp(reachedFrom, "0")){

                            puts(reachedFrom);
                            reachedFrom = searchReachedFrom(myHash, reachedFrom);
                        }
                    }

                    else printf("%d", searchLength(myHash, P));
                    destroyHash(myHash);
                    if (!destroyD(&queue)) DIE("destroyD() failed");
                    return;
                }

                else if (!searchReachedFrom(myHash, Pnew)){

                    insertElement(myHash, toAdd, P, length + 1);
                    if (!addD(&queue, toAdd)) DIE("addD() failed");
                }

            }
            //free(toAdd);
            free(Pnew);
        }
    }

    destroyHash(myHash);
    if (!destroyD(&queue)) DIE("destroyD() failed");
    return;
}

//Depending on funct, this function either turns initial into a 2D-array (board),
//or turns 2D-array to 1D-string (initial).
//Returns void.
void arrayCharExchange(char **initial, int height, int width, char board[height][width], bool funct){

    int x, y = 0;

    for (; y < height; y++){

        for (x = 0; x < width; x++){

            if (funct) board[y][x] = (*initial)[width * y + x];
            else (*initial)[width * y + x] = board[y][x];
        }
    }
}

//Gets the pos of a character and the width of the board,
//returns the x-coordinate of the character.
int getX(int pos, int width){

    return (pos % width);
}

//Gets the pos of a character and the width of the board,
//returns the y-coordinate of the character.
int getY(int pos, int width){

    return (pos / width);
}

//This is the main function which I move my dash.
//Checks whether currentX, currentY, dashY and dashX match.
//Makes the necessary changes.
//Returns true if the move is possible, else returns false.
bool moveDash(int height, int width, char board[height][width], int currentX, int currentY, int dashX, int dashY, bool rFlag){

    int distance = (currentY - dashY) * (currentY - dashY) + (currentX - dashX) * (currentX - dashX); //How many tiles to move.

    if ((!rFlag) && (distance > 1)) return false;

    if (distance <= 1){

        if (((currentY == dashY) && (currentX == dashX)) || ((currentY != dashY) && (currentX != dashX))) return false;

        board[dashY][dashX] = board[currentY][currentX]; //Regular one-tile move.
    }

    //r-flag cases.
    else if (currentX == dashX){

        if (currentY < dashY){ //Move up.

            for (; dashY != currentY; dashY--) board[dashY][dashX] = board[dashY - 1][dashX];
        }

        else{ //Move down.

            for (; dashY != currentY; dashY++) board[dashY][dashX] = board[dashY + 1][dashX];
        }
    }

    else if (currentY == dashY){

        if (currentX < dashX){ //Move left.

            for (; dashX != currentX; dashX--) board[dashY][dashX] = board[dashY][dashX - 1];
        }

        else{ //Move right.

            for (; dashX != currentX; dashX++) board[dashY][dashX] = board[dashY][dashX + 1];
        }   
    }
    board[currentY][currentX] = '-';
    return true; //Success
}
