/*
	Naval Battle

    By Rafael Alves de Lima, Gabriel Kato Gomes, Lucas Daniel Da Cunha Moura,2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

// --- Constants ---
int MD = 0;                                         // Mode Game
const int LINE = 5;				                    // Line array size
const int COLUMN = 5;			                    // Column array size
const int MAX_RAND = 5;			                    // Maximum value for boat draw
const int Q_ARGS = 5;                               // Number of parameters passed at run time
const char LETTERS[5] = {'A', 'B', 'C', 'D', 'E'};  // The letters of the lines on the board

// --- Prototype ---
int argsOk(int, char *[]);
int receiveArgsValues(char *[], int *);
void errorMsg(int);
void initOcean(char [LINE][COLUMN]);
void submarinesIntoOcean(int [2], int [2], int [2], char [LINE][COLUMN]);
void showOcean(const char [LINE][COLUMN]);
void showInventory(int, int, int);
void shoot(char [3]);
int checkRight(int [2], int [2], int [2], char [3]);
void presentsWon(char [LINE][COLUMN], int, int *, int);
void presentsLost(char [LINE][COLUMN], int, int *, int);
void submarinesIntoOceanMode2(int [2], int [2], int [2], char [LINE][COLUMN]);
void showOceanMode2(char [LINE][COLUMN]);

int
main(int argc, char *argv[])
{
    int hits = 0;               // Qtdd of hited ships
    int attempts = 0;           // Num of attempts
    int sub1[2];                // Submarine 1
    int sub2[2];                // Submarine 2
    int sub3[2];                // Submarine 3
    int x = 0;                  // Turns the shot[0] into number
    int y = 0;                  // Turns the shot[1] into number
    int receivesHit = 0;        // Receives a value from the check right function
    int destroyers = 0;         // Number of destroyers on the board
    int answerArgsOk = 0;       // argsOk answer
    int torpedoes = 0;          // Num of torpedoes
    long int submarines = 3;    // Num of submarines
    char ocean[LINE][COLUMN];   // Board
    char shot[3];               // Receive shot position

    answerArgsOk = argsOk(argc, argv);

    if (answerArgsOk != 0)
    {
        errorMsg(answerArgsOk);
        return answerArgsOk;
    }

    MD = receiveArgsValues(argv, &torpedoes);

    // Mode PLAY
    if (MD == 1) 
    {
        initOcean(ocean);
        submarinesIntoOcean(sub1, sub2, sub3, ocean);
        printf("Mode: Play\n");

        for (; submarines != 0;)
        {
            if (torpedoes == 0)
                break;

            if (submarines == 0)
                break;

            showOcean(ocean);
            showInventory(destroyers, submarines, torpedoes);

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
            presentsWon(ocean, submarines, &torpedoes, attempts);

        else
            presentsLost(ocean, submarines, &torpedoes, attempts);
    }

    // Mode CORR
    else if (MD == 2)
    {
        initOcean(ocean);
        submarinesIntoOcean(sub1, sub2, sub3, ocean);
        printf("Mode: CORR\n");

        for (; submarines != 0;)
        {
            if (torpedoes == 0)
                break;

            if (submarines == 0)
                break;

            showOcean(ocean);
            showInventory(destroyers, submarines, torpedoes);

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
            presentsWon(ocean, submarines, &torpedoes, attempts);

        else
            presentsLost(ocean, submarines, &torpedoes, attempts);
    }

    else
        printf("This mode doesn't exist!!!\n");
    
    printf("\n");
    
    return 0;
}

/*
    argsOk
    ------
    Check that the arguments passed to the program are consistent with the expected arguments. 
    Returns an integer value.
    Return:
        * 3, More or less than 5 arguments;
        * 5, Different than -t or different than -m;
        * 7, If it is equal to or less than 0 or is a digit;
        * 9, Whether it's PLAY or CORR mode;
        * 0, Everything worked;
*/
int 
argsOk(int argc, char *argv[])
{
    // Number of arguments
    if (argc != Q_ARGS)
        return 3;

    if ((strcmp(argv[1], "-t") != 0) 
        && strcmp(argv[1], "-m") != 0)
        return 5;

    if ((strcmp(argv[1], "-m") != 0) 
        && strcmp(argv[1], "-t") != 0)
        return 5;

    // If it's -m or -t
    if ((strcmp(argv[1], "-t") == 0)
        && (strcmp(argv[3], "-m") != 0))
            return 5;

    // If it's -m or -t
    if ((strcmp(argv[1], "-m") == 0)
        && (strcmp(argv[3], "-t") != 0))
            return 5;
    
    // Check number and mode
    if (strcmp(argv[1], "-t") == 0)
    {
        if (atoi(argv[2]) <= 0)
            return 7;
        
        if (strcmp(argv[3], "-m") == 0)
        {
            if ((strcmp(argv[4], "PLAY") != 0)
                && (strcmp(argv[4], "CORR") != 0))
                    return 9;
        }
    }

    // Check number and mode
    if (strcmp(argv[1], "-m") == 0)
    {
        if (strcmp(argv[3], "-t") == 0)
            if (atoi(argv[4]) <= 0)
                return 7;

        if ((strcmp(argv[2], "PLAY") != 0)
            && (strcmp(argv[2], "CORR") != 0))
                return 9;
    }

    // Everything ended right
    else
        return 0;
}

/*
    errorMsg
    --------
    Shows error messages from passed arguments.
*/
void
errorMsg(int x)
{
    switch (x)
    {
        case 3:
            fprintf(stderr, "Too many or too few arguments\n");
            break;
        
        case 5:
            fprintf(stderr, "Invalid label\n");
            break;
        
        case 7:
            fprintf(stderr, "It is not a number or <= 0\n");
            break;
            
        case 9:
            fprintf(stderr, "Unknown game mode\n");
            break;
        
        default:
            fprintf(stderr, "Unknown error code\n");
            break;
    }

    printf("<prog> -t <value> -m <value>\n");
}

/*
    receiveArgsValues
    -----------------
    Returns the values of -t and mode -m to main.
    To find out which mode and quantity torpedoes.
    Returns:
        * mg = 1, mode PLAY;
        * mg = 2, mode CORR;
*/
int
receiveArgsValues(char *argv[], int *torpedoes)
{
    int mg = 0;     // Mode Game

    if (strcmp(argv[1], "-t") == 0)
    {
        *torpedoes = atoi(argv[2]);
        
        if (strcmp(argv[3], "-m") == 0)
        {
            if (strcmp(argv[4], "PLAY") == 0)
                mg = 1;
            
            else if (strcmp(argv[4], "CORR") == 0)
                mg = 2;
        }
    }

    else if (strcmp(argv[1], "-m") == 0)
    {
        if (strcmp(argv[2], "PLAY") == 0)
            mg = 1;    
        
        else if (strcmp(argv[2], "CORR") == 0)
            mg = 2; 

        if (strcmp(argv[3], "-t") == 0)
            *torpedoes = atoi(argv[4]);
    }  

    return mg;
}

/*
	initOcean
	----------
	Gerenate the lines and collumns of the board.
    Starting all rows and columns with -1.
*/
void
initOcean(char ocean[LINE][COLUMN])
{
    int l;
    int c;

    for (l = 0; l < LINE; l++)
        for (c = 0; c < COLUMN; c++)
            ocean[l][c] = -1;
}

/*
	submarinesIntoOcean
	----------
	Generates the position of the ships on the board.
	The positon of the ships is generated randomly.
    Always with a sector of difference.
*/
void
submarinesIntoOcean(int sub1[2], int sub2[2], int sub3[2], char ocean[LINE][COLUMN])
{
    int random = 0;     // Random numbers result 
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

    if (MD == 2)
    {
        ocean[sub1[0]][sub1[1]] = 1;
        ocean[sub2[0]][sub2[1]] = 1;
        ocean[sub3[0]][sub3[1]] = 1;  
    }
}

/*
	showOcean
	---------
	Shows the ocean on screen.
    The symbols mean:
        * W, means water;
        * *, means submarine position.
*/
void
showOcean(const char ocean[LINE][COLUMN])
{
    int l;
    int c;

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
}

/*
    showInventory
    -------------
    Display inventory on the destroyers(d), submarines(s) and torpedoes(t) screen.
*/
void 
showInventory(int d, int s, int t)
{
    printf("\nDestroyers: %d\n", d);
    printf("Submarines: %d\n", s);
    printf("Torpedoes: %d\n", t);
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
        * 1, if you didn't hit the water.
*/
int
checkRight(int sub1[2], int sub2[2], int sub3[2], char shot[3])
{
    int x = (int) shot[0];
    int y = (int) shot[1] - 49;

    if ((x == sub1[0]) && (y == sub1[1]))
    {
        printf("\nYou sunk a submarine\n");
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
presentsWon(char ocean[LINE][COLUMN], int submarines, int *torpedoes, int attempts)
{
    system("clear");
    showOcean(ocean);
    printf("\nYou won!!!\n");
    printf("Remaining submarines: %d\n", submarines);
    printf("Remaining torpedoes: %d\n", *torpedoes);
    printf("Attempts: %d\n", attempts);
}

/*
    presentsLost
    -----------
    Message displayed at the end of the program if you lose the game.
*/
void
presentsLost(char ocean[LINE][COLUMN], int submarines, int *torpedoes, int attempts)
{
    system("clear");
    showOcean(ocean);
    printf("\nYou lost!!!\n");
    printf("Remaining submarines: %d\n", submarines);
    printf("Remaining torpedoes: %d\n", *torpedoes);
    printf("Attempts: %d\n", attempts);
}
