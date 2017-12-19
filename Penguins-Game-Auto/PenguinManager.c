#include <stdio.h>
#include "PenguinManager.h" 

extern int currentPlayer;
extern int playersScores[10]; //1,2,3,... not 0,1,2,3,...
extern int numberOfPenguinPerPlayer;

extern int numberOfMapRows;
extern int numberOfMapColumns;
extern char map[20][20];

extern int tilesTakenThisTurn[10][2]; //Places where penguins were moved in this turn
extern int numberOfPenguinsBlocked; //Penguins permananently blocked in this turn
int numberOfPenguinsMovedThisTurn = 0;

//--------------------------PLACEMENT---------------------------

void PlacePenguin() //PLACEMENT
{
	int r, c; //Randomize coordinates for penguin position
	r = rand() % (numberOfMapRows - 1); 
	c = rand() % (numberOfMapColumns - 1);

	if (map[r][c] - '0' != 1) //Invalid tile (taken, flooded or not 1 fish)
	{
		//printf("Failed placing penguin on %d,%d\n", r, c); //[DEBUG]
		PlacePenguin(); //Try again
	}
	else
	{
		map[r][c] = CurrentPlayerPenguinLetter(1); //Place penguin
		//printf("Penguin placed on %d,%d\n", r, c); //[DEBUG]
	}
}

//--------------------------MOVEMENT----------------------------

void FindPenguinAndMove(int row, int col)
{
	while (numberOfPenguinsMovedThisTurn < numberOfPenguinPerPlayer)
	{
		//Scan rows for non moved penguins
		int r; for (r = row; r < numberOfMapRows; r++)
		{
			int c; for (c = col; c < numberOfMapColumns; c++)
			{
				if (map[r][c] == CurrentPlayerPenguinLetter(1) || map[r][c] == CurrentPlayerPenguinLetter(2) || map[r][c] == CurrentPlayerPenguinLetter(3)) //Finding penguin that belongs to current player on map 
				{
					int checks = 0;
					int i; for (i = 0; i < 10; i++) //Checking in tilesTakenThisTurn array if this penguin wasn't already moved in this turn
					{										
						if (tilesTakenThisTurn[i][0] == r && tilesTakenThisTurn[i][1] == c) { checks++; }
					}			
					if (checks < 1) { TryToMovePenguin(r, c); } //Found penguin that wasn't already moved in this turn - try to move
				}				
			}
		}
	}
}

void TryToMovePenguin(int r, int c) //Try to find free neighbour tile for penguin on given coordinates (Randomize 1 of 6 - 25 chances for finding good one, otherwise penguin is assumed as blocked)
{
	int iterations = 0;
	while (iterations < 25)
	{
		int randomNeighbourTile = rand() % 6 + 1; //Randomize 1 of 6 possible neighbour tiles 
		int newTileR = 0;
		int newTileC = 0;
		switch (randomNeighbourTile) //Coordinates of neighbour tiles relative to the penguin position
		{
		case 1:
			newTileR = r - 1;
			newTileC = c + 1;
			break;
		case 2:
			newTileR = r;
			newTileC = c + 1;
			break;
		case 3:
			newTileR = r + 1;
			newTileC = c + 1;
			break;
		case 4:
			newTileR = r + 1;
			newTileC = c;
			break;
		case 5:
			newTileR = r;
			newTileC = c - 1;
			break;
		case 6:
			newTileR = r - 1;
			newTileC = c;
			break;
		}
		if (map[newTileR][newTileC] > 48 && map[newTileR][newTileC] < 52) //Check if there is a fish on randomized neighbour tile 
		{ 
			MovePenguinToNewTile(r, c, newTileR, newTileC);
			break;
		}
		else //If no, randomize tile again until 25 failed attempts
		{
			iterations++; 
		}		
	}
	if (iterations == 25) //25 failed attempts. Couldn't find free neighbour tile for this penguin. Penguin is blocked
	{
		tilesTakenThisTurn[numberOfPenguinsMovedThisTurn][0] = r; //Add new taken tile
		tilesTakenThisTurn[numberOfPenguinsMovedThisTurn][1] = c; //Add new taken tile
		numberOfPenguinsMovedThisTurn++; //Increase number of penguins moved this turn (or tried to move)
		numberOfPenguinsBlocked++; //Increase number of blocked penguins in this turn
		//printf("Penguin on %d,%d is blocked\n", r, c); //[DEBUG]
	}	
	//int i; for (int i = 0; i < 4; i++) { printf("TilesTaken - %d,%d\n", tilesTakenThisTurn[i][0], tilesTakenThisTurn[i][1]); } //[DEBUG]
}

void MovePenguinToNewTile(int currentTileR, int currentTileC, int newTileR, int newTileC)
{
	playersScores[currentPlayer] += FishesCountFromLetter(map[currentTileR][currentTileC]); //Add points to score
	map[newTileR][newTileC] = CurrentPlayerPenguinLetter(map[newTileR][newTileC] - '0');  //Place penguin on new tile
	map[currentTileR][currentTileC] = ' '; //Flood tile
	tilesTakenThisTurn[numberOfPenguinsMovedThisTurn][0] = newTileR; //Add new taken tile
	tilesTakenThisTurn[numberOfPenguinsMovedThisTurn][1] = newTileC; //Add new taken tile
	numberOfPenguinsMovedThisTurn++; //Increase number of penguins moved this turn (or tried to move)
	//printf("Penguin moved from %d,%d to %d,%d\n", currentTileR, currentTileC, newTileR, newTileC); //[DEBUG]
}

//------------------------OTHER-FUNCTIONS---------------------------

char CurrentPlayerPenguinLetter(int fishesOnTile) //Returns letter that mark penguin on map depending on fishes number and current player
{
	switch (currentPlayer)
	{
	case 1: //For player 1
		if (fishesOnTile == 1) { return 'a'; }
		if (fishesOnTile == 2) { return 'A'; }
		if (fishesOnTile == 3) { return 'U'; }
		break;
	case 2: //For player 2
		if (fishesOnTile == 1) { return 'b'; }
		if (fishesOnTile == 2) { return 'B'; }
		if (fishesOnTile == 3) { return 'V'; }
		break;
	case 3: //For player 3
		if (fishesOnTile == 1) { return 'c'; }
		if (fishesOnTile == 2) { return 'C'; }
		if (fishesOnTile == 3) { return 'W'; }
		break;
	case 4: //For player 4
		if (fishesOnTile == 1) { return 'd'; }
		if (fishesOnTile == 2) { return 'D'; }
		if (fishesOnTile == 3) { return 'X'; }
		break;
	case 5: //For player 5
		if (fishesOnTile == 1) { return 'e'; }
		if (fishesOnTile == 2) { return 'E'; }
		if (fishesOnTile == 3) { return 'Y'; }
		break;
	case 6: //For player 6
		if (fishesOnTile == 1) { return 'f'; }
		if (fishesOnTile == 2) { return 'F'; }
		if (fishesOnTile == 3) { return 'Z'; }
		break;
	}
	return '#';
}

int FishesCountFromLetter(char letter) //Returns number of fishes on tile depending on penguin that stands on it
{
	if (letter > 96 && letter < 103) { return 1; } //For letters: a, b, c, d, e, f
	if (letter > 64 && letter < 71) { return 2; } //For letters: A, B, C, D, E, F
	if (letter > 84 && letter < 91) { return 3; } //For letters: U, V, W, X, Y, Z
	return 0;
}

void CreateTilesTakenThisTurnArray() //Fills array of tiles taken by penguins in this turn with initail numbers 
{
	for (int i = 0; i < 10; i++) 
	{ 
		tilesTakenThisTurn[i][0] = -1; 
		tilesTakenThisTurn[i][1] = -1;
	}
}