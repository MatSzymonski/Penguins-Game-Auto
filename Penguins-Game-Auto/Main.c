//Max 6 players
//Max 10 penguins per player
//25 chances to find correct neighbour tile out of 6 (TryToMovePenguin function)
//20 failed turns to game end (main function)

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
 
int numberOfPenguinsBlocked = 0; //Penguins permananently blocked in this turn

int failedTurnsIterations; //When all penguins failed to move
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
		ReadIterationsFile(); //Read failedTurnsIterations from separate file (function in IOFileManager.c)
		CheckGamePhase();
	
		if (currentPlayer == 0 || currentPlayer > numberOfPlayers) { currentPlayer = 1; } //Fixing improper currentPlayer variable
																						  //PrintDataFile(); //[DEBUG]
		if (strncmp(gamePhase, "placement", 9) == 0) //PLACEMENT
		{
			PlacePenguin();
		}
		else //MOVEMENT
		{
			CreateTilesWithCurrentPlayerPenguinsArray();
			FindAllCurrentPlayerPenguins();	printf("1");
			if (numberOfPenguinsBlocked == numberOfPenguinPerPlayer) //If all penguins are blocked add one more to failedTurnsIterations else move penguin
			{
				failedTurnsIterations++;
				if (failedTurnsIterations > 20) { FinishTheGame(); return 0; } //Finish the game when cant move 100th time
			}
			else
			{
				MovePenguin();
			}
		}
		IncreaseCurrentPlayerIndex(); //Increase current player index
		WriteDataToOutputFile(); //Create output file (function in IOFileManager.c)
		WriteIterationsFile(); //Write failedTurnsIterations to separate file (function in IOFileManager.c)							 		
	

	return 0;
}

//------------------------OTHER-FUNCTIONS---------------------------

void FinishTheGame() 
{
	//Create sorted array
	int sortedPlayersScores[10];
	int x; for (x = 0; x < numberOfPlayers + 1; x++)
	{
		sortedPlayersScores[x] = playersScores[x];
	}

	//Sort scores array
	int i, j, temp;
	for (i = 1; i < numberOfPlayers + 1; i++)
	{
		for (j = i + 1; j < numberOfPlayers + 1; j++)
		{
			if (sortedPlayersScores[i] < sortedPlayersScores[j])
			{
				temp = sortedPlayersScores[i];
				sortedPlayersScores[i] = sortedPlayersScores[j];
				sortedPlayersScores[j] = temp;
			}
		}
	}

	//Display results
	printf("GAME OVER\nResults:\n");
	int z; for (z = 1; z < numberOfPlayers + 1; z++) 
	{	
		printf("%d: Player %d - %d points\n", z, IndexOfPlayerWithScore(sortedPlayersScores[z]), sortedPlayersScores[z]);
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
	int i; for (i = 1; i < numberOfPlayers+1; i++)
	{
		if (playersScores[i] == score)
		{
			return i;
		}
	}
	return 0;
}

