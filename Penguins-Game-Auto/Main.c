//Max 6 players
//Max 10 penguins per player
//25 chances to find correct neighbour tile out of 6 (TryToMovePenguin function)

/* TO DO 
- ITERATIONS FILE
- CHECK IF PRINTING WINNER WORKS
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "IOFileManager.h" //Header with reading and writing IO file functions
#include "PenguinManager.h" //Header with placing and moving penguins functions

//Data from input file
int currentPlayer;
int playersScores[10];  //1,2,3,... not 0,1,2,3,...
int numberOfPlayers;
int numberOfPenguinPerPlayer;
int numberOfPlacedPenguins;
char gamePhase[25];

int numberOfMapRows;
int numberOfMapColumns;
char map[20][20];

int tilesTakenThisTurn[10][2]; //Places where penguins were moved in this turn
int failedTurnsIterations = 0; //When all penguins failed to move
int numberOfPenguinsBlocked = 0; //Penguins permananently blocked in this turn

//-------------------FUNCTIONS-DECLARATION------------------

void FinishTheGame();
void SetUpRandomizer();
void IncreaseCurrentPlayerIndex();
void CheckGamePhase();
int IndexOfPlayerWithScore(int);
void StrCopy(char*, char*);

//--------------------------MAIN----------------------------
int main()
{	
	SetUpRandomizer();
	ReadDataFromInputFile(); //Read data from InputFile (function in IOFileManager.c)
	CheckGamePhase();

	if (currentPlayer == 0 || currentPlayer > numberOfPlayers) { currentPlayer = 1; } //Fixing improper currentPlayer variable
	//PrintDataFile(); //[DEBUG]
	
	if (strncmp(gamePhase, "placement", 9) == 0) //PLACEMENT
	{	
		PlacePenguin();	
	}
	else //MOVEMENT
	{
		CreateTilesTakenThisTurnArray();
		FindPenguinAndMove(0,0);
	}

	if (numberOfPenguinsBlocked == numberOfPenguinPerPlayer) { failedTurnsIterations++; } //Check if all penguins are blocked
	if (failedTurnsIterations > 100) { FinishTheGame(); return 0; } //Finish the game when cant move 100th time

	IncreaseCurrentPlayerIndex(); //Increase current player index
	WriteDataToOutputFile(); //Create output file (function in IOFileManager.c)
								 
	return 0;
}

//------------------------OTHER-FUNCTIONS---------------------------

void FinishTheGame() 
{
	//Create sorted array
	int sortedPlayersScores[10];
	int x; for (x = 0; x < numberOfPlayers; x++)
	{
		sortedPlayersScores[x] = playersScores[x];
	}

	//Sort scores array
	int i, j, temp;
	for (i = 1; i < numberOfPlayers + 1; i++)
	{
		for (j = i + 2; j < numberOfPlayers + 1; j++)
		{
			if (sortedPlayersScores[i] > sortedPlayersScores[j])
			{
				temp = sortedPlayersScores[i];
				sortedPlayersScores[i] = sortedPlayersScores[j];
				sortedPlayersScores[j] = temp;
			}
		}
	}

	//Display results
	int z; for (z = 1; z < numberOfPlayers + 1; z++) 
	{
		printf("GAME OVER\nResults:\n");
		printf("%d: Player %d - %d points", z, IndexOfPlayerWithScore(sortedPlayersScores[z]), sortedPlayersScores[z]);
	}
}

void SetUpRandomizer() 
{
	int seed;
	time_t tt;
	seed = time(&tt);
	srand(seed);
}

void IncreaseCurrentPlayerIndex()
{
	if (currentPlayer == numberOfPlayers)
	{
		currentPlayer = 1;
	}
	else
	{
		currentPlayer++;
	}
}

void CheckGamePhase()
{
	if (strncmp(gamePhase, "placement", 9) == 0) 
	{
		if (numberOfPlacedPenguins >= numberOfPlayers * numberOfPenguinPerPlayer) //Change game phase if all players placed their penguins
		{
			StrCopy("movement ", gamePhase);
		}
	}
}

int IndexOfPlayerWithScore(int score) //Finding index of player with given score
{
	int i; for (int i = 0; i < numberOfPlayers; i++)
	{
		if (playersScores[i] == score)
		{
			return i;
		}
	}
	return 0;
}

void StrCopy(char* str_1, char* str_2) //String copy
{
	while (*str_1 != '\0')
	{
		*str_2 = *str_1++;
		++str_2;
	}
}

