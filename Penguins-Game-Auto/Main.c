#include <stdio.h>
#include "IOFileManager.h" //Header with reading and writing IO file functions

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


//Functions declaration
void IncreaseCurrentPlayerIndex();
void StrCopy(char*, char*);

int main() //MATEUSZ SZYMOÑSKI
{
	ReadDataFromInputFile(); //Read data from InputFile (function in IOFileManager.c)

	//int i; for (i = 0; i < numberOfMapRows; i++) { printf("%s", map[i]); } //PRINT THE MAP(map); Extend this 
	PrintDataFile();

	if (gamePhase == "placement")
	{

		//PLACE PENGUIN(p);


		if (numberOfPlacedPenguins >= numberOfPlayers * numberOfPenguinPerPlayer) //Change game phase
		{
			StrCopy("movement\n", gamePhase);
		}
	}
	else if (gamePhase == "movement")
	{
		int p;
		for (p = 0; p < numberOfPenguinPerPlayer; p++)
		{
			int ways; //= FIND POSSIBLE WAYS OF MOVEMENT FOR PENGUIN();
			if (ways > 0)
			{
				//FIND THE BEST WAY FOR PENGUIN(p);
				//MOVE PENGUIN(p); (1. Collect fish from the current tile(change playerScores[] 2. Place penguin on next tile(change mapRows[][] data))
			}
			else
			{
				//penguin is blocked - move next penguin or end this turn if all other penguins were moved already
			}
		}
	}

	/*
	!!!I/O File need to be extended to store what players doesn't have movement possibility!!!
	if(If none of the penguins of all players have possibility to move)
	{
	PRINT ALL SCORES AND PICK THE WINNER();
	}
	*/

	IncreaseCurrentPlayerIndex(); //Increase current player index (Read David's concept)
								  //CREATE OUTPUT FILE(); (function in IOFileManager.c)
	return 0;

}


void IncreaseCurrentPlayerIndex() //MATEUSZ SZYMOÑSKI
{
	if (currentPlayer == 0)
	{
		currentPlayer = 2; //Double incremantation (Read David's concept)
	}
	else
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
}

void StrCopy(char* str_1, char* str_2)
{
	while (*str_1 != '\0')
	{
		*str_2 = *str_1++;
		++str_2;
	}
}