//--------------------MATEUSZ-SZYMONSKI-----------------------

#include <stdio.h>
#include "PenguinManager.h" 

extern int currentPlayer;
extern int playersScores[10]; //1,2,3,... not 0,1,2,3,...
extern int numberOfPenguinPerPlayer;

extern int numberOfMapRows;
extern int numberOfMapColumns;
extern char map[20][20];

int tilesWithCurrentPlayerPenguins[10][3]; //10 slots for penguins | 0 - (0 = non blocked, 1 = blocked), 1 - row number, 2 - column number 
extern int numberOfPenguinsBlocked; //Penguins permananently blocked in this turn

int newTileR;
int newTileC;

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

void FindAllCurrentPlayerPenguins() //Finding penguin that belongs to current player on map and storing their coordinates in array
{
	int penguinsFound = 0;
	int r; for (r = 0; r < numberOfMapRows; r++)
	{
		int c; for (c = 0; c < numberOfMapColumns; c++)
		{
			if (map[r][c] == CurrentPlayerPenguinLetter(1) || map[r][c] == CurrentPlayerPenguinLetter(2) || map[r][c] == CurrentPlayerPenguinLetter(3))
			{
				tilesWithCurrentPlayerPenguins[penguinsFound][0] = CheckIfIsBlocked(r, c); //Check if is blocked or not
				tilesWithCurrentPlayerPenguins[penguinsFound][1] = r;
				tilesWithCurrentPlayerPenguins[penguinsFound][2] = c;
				penguinsFound++;	//printf("TEST");
			}
		}
	}
}

void MovePenguin() //If non blocked move. If blocked random once again
{
	int randomPenguin = rand() % numberOfPenguinPerPlayer; //Randomize 1 penguin
	if (tilesWithCurrentPlayerPenguins[randomPenguin][0] == 0) 
	{
		RandomizeTileAndChangeMap(tilesWithCurrentPlayerPenguins[randomPenguin][1], tilesWithCurrentPlayerPenguins[randomPenguin][2]); //Move
	}
	else
	{
		MovePenguin(); //Draw one more time
	}
}

int CheckIfIsBlocked(int r, int c) //Try to find free neighbour tile for penguin on given coordinates (Randomize 1 of 6 - 25 chances for finding good one, otherwise penguin is assumed as blocked)
{
	int iterations = 0;
	while (iterations < 25)
	{
	    RandomizeNeighbourTile(r, c);//printf("TEST"); ///////////////////
		if (map[newTileR][newTileC] > 48 && map[newTileR][newTileC] < 52) //Check if there is a fish on randomized neighbour tile 
		{			
		    return 0; //non blocked			
		}
		else //If no, randomize tile again until 25 failed attempts
		{
			iterations++;
		}
	}
	if (iterations == 25) //25 failed attempts. Couldn't find free neighbour tile for this penguin. Penguin is blocked
	{
		numberOfPenguinsBlocked++; //Increase number of blocked penguins in this turn	
		//printf("Penguin on %d,%d is blocked\n", r, c); //[DEBUG]
		return 1; //Blocked		
	}	
}

void RandomizeTileAndChangeMap(int r, int c) //Find free neighbour tile for penguin on given coordinates
{
    RandomizeNeighbourTile(r, c);
	if (map[newTileR][newTileC] > 48 && map[newTileR][newTileC] < 52) //If there is a fish on randomized neighbour tile
	{
		ChangeMap(r, c, newTileR, newTileC); //Move penguin to this tile
	}
	else 
	{
		RandomizeTileAndChangeMap(r, c); //Randomize tile again
	}
}

void ChangeMap(int currentTileR, int currentTileC, int newTileR, int newTileC)
{
	playersScores[currentPlayer] += FishesCountFromLetter(map[currentTileR][currentTileC]); //Add points to score
	map[newTileR][newTileC] = CurrentPlayerPenguinLetter(map[newTileR][newTileC] - '0');  //Place penguin on new tile
	map[currentTileR][currentTileC] = ' '; //Flood tile
}

//------------------------OTHER-FUNCTIONS---------------------------
void RandomizeNeighbourTile(int r, int c)
{
	int randomNeighbourTile = rand() % 6 + 1; //Randomize 1 of 6 possible neighbour tiles 
	newTileR = 0;
	newTileC = 0;
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
}

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

void CreateTilesWithCurrentPlayerPenguinsArray() //Fills array of tiles taken by penguins in this turn with initail numbers 
{
	int i; for (i = 0; i < numberOfPenguinPerPlayer; i++)
	{
		tilesWithCurrentPlayerPenguins[i][0] = -1;
		tilesWithCurrentPlayerPenguins[i][1] = -1;
		tilesWithCurrentPlayerPenguins[i][2] = -1;
	}
}