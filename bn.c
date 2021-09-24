/*
	Naval Battle

    By Rafael Alves de Lima, Gabriel Kato Gomes, Nicole Aparecida Oliveira, 2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// --- Constants ---
enum modes {PLAY, CORR};		                    // Mode of game playing
const int MD = CORR;                                // Game Mode
const int LINE = 5;				                    // Line array size
const int COLUMN = 5;			                    // Column array size
const int MAX_RAND = 5;			                    // Maximum value for boat draw
const char LETTERS[5] = {'A', 'B', 'C', 'D', 'E'};  // The letters of the lines on the board

// --- Prototype ---
void startBoard(int [LINE][COLUMN]);
void startShips(int [2], int [2], int [2]);
void showBoard(int [LINE][COLUMN], int , int, int);
void shoot(char [3]);
int checkRight(int [2], int [2], int [2], char [3]);
void presentsWon(int [LINE][COLUMN], int , int , int , int);
void presentsLost(int [LINE][COLUMN], int , int , int , int);
void startShipsMode2(int [2], int [2], int [2], int [LINE][COLUMN]);
void showBoardMode2(int [LINE][COLUMN]);

int
main()
{
    int ocean[LINE][COLUMN];    // Board
    int hits = 0;               // Qtdd of hited ships
    int attempts = 0;           // Num of attempts
    int sub1[2];                // Submarine 1
    int sub2[2];                // Submarine 2
    int sub3[2];                // Submarine 3
    int x = 0;                  // Turns the shot[0] into number
    int y = 0;                  // Turns the shot[1] into number
    int receivesHit = 0;        // Receives a value from the check right function
    int destroyers = 0;         // Number of destroyers on the board
    long int torpedoes = 6;     // Num of torpedoes
    long int submarines = 3;    // Num of submarines
    char shot[3];               // Receive shot position

    if (MD == PLAY)
    {
        startBoard(ocean);
        startShips(sub1, sub2, sub3);
        printf("Mode: Play\n");

        for (; submarines != 0;)
        {
            if (torpedoes == 0)
                break;

            if (submarines == 0)
                break;

            showBoard(ocean, submarines, torpedoes, destroyers);

            shoot(shot);

            torpedoes--;

            x = (int) shot[0];
            y = (int) shot[1] - 49;

            receivesHit = checkRight(sub1, sub2, sub3, shot);

            // Hit a boat
            if (receivesHit == 0) 
            {
                submarines--;
                ocean[x][y] = 1;
            }  

            // Missed the boat
            else if (receivesHit == 1)
                ocean[x][y] = 0;    

            printf("-------------------------------------\n");

            attempts++;
        }

        if (hits == submarines)
            presentsWon(ocean, submarines, torpedoes, attempts, destroyers);

        else
            presentsLost(ocean, submarines, torpedoes, attempts, destroyers);
    }

    else if (MD == CORR)
    {
        startBoard(ocean);
        startShipsMode2(sub1, sub2, sub3, ocean);
        printf("Mode: Corr\n");
        showBoardMode2(ocean);
    }

    else
        printf("This mode doesn't exist!!!\n");
    
    printf("\n");
    
    return 0;
}

/*
	startBoard
	----------
	Gerenate the lines and collumns of the board.
    Starting all rows and columns with -1.
*/
void
startBoard(int ocean[LINE][COLUMN])
{
    int l, c;

    for (l = 0; l < LINE; l++)
        for (c = 0; c < COLUMN; c++)
            ocean[l][c] = -1;
}

/*
	startShips
	----------
	Generates the position of the ships on the board.
	The positon of the ships is generated randomly.
    Always with a sector of difference.
*/
void
startShips(int sub1[2], int sub2[2], int sub3[2])
{
    int random = 0; // Random numbers result 
    int i;

    for (i = 0; i < 2; i++)
        sub1[i] = rand() % MAX_RAND;
    
    for (i = 0; i < 2; i++)
    {
        random = rand() % MAX_RAND;

        if (abs(random - sub1[i]) > 1)
            sub2[i] = random;
        
        else
            i--;
    }

    for (i = 0; i < 2; i++)
    {
        random = rand() % MAX_RAND;

        if (abs(random - sub1[i]) > 1)
            sub3[i] = random;

        else if (abs(random - sub2[i]) > 1)
            sub3[i] = random;
        
        else
            i--;
    }
}

/*
	showboard
	---------
	Shows the ocean on screen and basic information.
    The symbols mean:
        W , means water;
        * , means submarine position.
*/
void
showBoard(int ocean[LINE][COLUMN], int submarines, int torpedoes, int destroyers)
{
    int l, c;

    printf("\n    1   2   3   4   5\n");

    for (l = 0; l < LINE; l++)
    {
		printf("  ");

        for (c = 0; c < COLUMN; c++)
        {
			printf("+---");
			
			if(c == 4)
			    printf("+");
        }

        printf("\n");

        if (l == 0)
            printf("%c", LETTERS[0]);

        if (l == 1)
            printf("%c", LETTERS[1]);
        
        if (l == 2)
            printf("%c", LETTERS[2]);
        
        if (l == 3)
            printf("%c", LETTERS[3]);
        
        if (l == 4)
            printf("%c", LETTERS[4]);

        for (c = 0; c < COLUMN; c++)
        {   
            if (ocean[l][c] == -1)
                printf(" |  ");

            else if (ocean[l][c] == 0)
                printf(" | W");
            
            else if (ocean[l][c] == 1)
                printf(" | *");
        }
	
        if (l < 5)
            printf(" |");
        
        printf("\n");
        
        if (l == 4)
            printf("  +---+---+---+---+---+");
    }

    printf("\n\nDestroyers: %d\n", destroyers);
    printf("Submarines: %d\n", submarines);
    printf("torpedoes: %d\n", torpedoes);
}

/*
	shoot
	-----
	Represents the launch action of a torpedo.
    Transforms the shot position [0] into an integer using the ASCII table.
*/
void
shoot(char shot[3])
{
    int l, c;

    printf("\n");
    printf("Shooting position: ");
    scanf("%s", shot);

    if (shot[0] == 65)
        shot[0] = 0;

    else if (shot[0] == 66)
        shot[0] = 1;
    
    else if (shot[0] == 67)
        shot[0] = 2;
    
    else if (shot[0] == 68)
        shot[0] = 3;
    
    else if (shot[0] == 69)
        shot[0] = 4;
}

/*
	checkRight
	----------
	Check if the shot hit a submarine or not.
	The function returns:
        * 0, if a submarine was sank.
        * 1, if not.
*/
int
checkRight(int sub1[2], int sub2[2], int sub3[2], char shot[3])
{
    int i;
    int x = (int) shot[0];
    int y = (int) shot[1] - 49;

    if ((x == sub1[0]) && (y == sub1[1]))
    {
        printf("\nHit the sub1\n");
        return 0;
    }

    else if ((x == sub2[0]) && (y == sub2[1]))
    {
        printf("\nYou sunk a submarine\n");
        return 0;
    }

    else if ((x == sub3[0]) && (y == sub3[1]))
    {
        printf("\nYou sunk a submarine\n");
        return 0;
    }

    // Missed the submarine
    else
    {
        printf("\nHit the water\n");
        return 1;
    }
}

/*
    presentsWon
    -----------
    Message displayed at the end of the program if you win the game.
*/
void
presentsWon(int ocean[LINE][COLUMN], int submarines, int torpedoes, int attempts, int destroyers)
{
    system("clear");
    showBoard(ocean, submarines, torpedoes, destroyers);
    printf("\nYou won!!!\n");
    printf("Remaining submarines: %d\n", submarines);
    printf("Attempts: %d\n", attempts);
}

/*
    presentsLost
    -----------
    Message displayed at the end of the program if you lose the game.
*/
void
presentsLost(int ocean[LINE][COLUMN], int submarines, int torpedoes, int attempts, int destroyers)
{
    system("clear");
    showBoard(ocean, submarines, torpedoes, destroyers);
    printf("\nYou lost!!!\n");
    printf("Remaining submarines: %d\n", submarines);
    printf("Attempts: %d\n", attempts);
}

/*
	startShipsMode2
	---------------
	Generates the position of ships on the board in the game's corrosion mode.
*/
void
startShipsMode2(int sub1[2], int sub2[2], int sub3[2], int ocean[LINE][COLUMN])
{
    int random = 0; // Random numbers result 
    int i;

    for (i = 0; i < 2; i++)
        sub1[i] = rand() % MAX_RAND;

    for (i = 0; i < 2; i++)
    {
        random = rand() % MAX_RAND;

        if (abs(random - sub1[i]) > 1)
            sub2[i] = random;
        
        else
            i--;
    }

    for (i = 0; i < 2; i++)
    {
        random = rand() % MAX_RAND;

        if (abs(random - sub1[i]) > 1)
            sub3[i] = random;

        else if (abs(random - sub2[i]) > 1)
            sub3[i] = random;
        
        else
            i--;
    }

    ocean[sub1[0]][sub1[1]] = 1;
    ocean[sub2[0]][sub2[1]] = 1;
    ocean[sub3[0]][sub3[1]] = 1;    
}

/*
	showBoardMode2
	--------------
	Shows the ocean with all the positions of the submarines on the screen.
	The symbols mean:
        W , means water;
        * , means submarine position.
*/
void
showBoardMode2(int ocean[LINE][COLUMN])
{
    int l, c;

    printf("\n    1   2   3   4   5\n");

    for (l = 0; l < LINE; l++)
    {
		printf("  ");

        for (c = 0; c < COLUMN; c++)
        {
			printf("+---");
			
			if(c == 4)
			    printf("+");
        }

        printf("\n");

        if (l == 0)
            printf("%c", LETTERS[0]);

        if (l == 1)
            printf("%c", LETTERS[1]);
        
        if (l == 2)
            printf("%c", LETTERS[2]);
        
        if (l == 3)
            printf("%c", LETTERS[3]);
        
        if (l == 4)
            printf("%c", LETTERS[4]);

        for (c = 0; c < COLUMN; c++)
        {           
            if (ocean[l][c] == -1)
                printf(" | W");
            
            else if (ocean[l][c] != -1)
                printf(" | *");
        }
	
        if (l < 5)
            printf(" |");
        
        printf("\n");
        
        if (l == 4)
            printf("  +---+---+---+---+---+");
    }

	printf("\n\n");
}
