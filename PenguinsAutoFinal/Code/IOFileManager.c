//------------------------MICHAL-KURCZAK-----------------------------

#include <stdio.h>
#include <string.h>
#include "IOFileManager.h" 

extern int currentPlayer;
extern int playersScores[10]; //1,2,3,... not 0,1,2,3,...
extern int numberOfPlayers;
extern int numberOfPenguinPerPlayer;
extern int numberOfPlacedPenguins;
extern char gamePhase[25];
extern int numberOfMapRows;
extern int numberOfMapColumns;

extern char map[20][20];

extern int failedTurnsIterations; //When all penguins failed to move

char scoresRow[25]; //first row of datafile as string

//---------------------READING-FROM-I/O-FILE---------------------------

void ReadDataFromInputFile(char *path) //Loop through all lines of the InputFile and assign data to the variables
{
	int line = 0;
	numberOfMapRows = 0;
	FILE* inputFile = fopen(path, "r"); //Open the file for reading
	if (inputFile == NULL) { printf("Error opening data file\n"); } //Check if the file has been opened
	//else { printf("Success opening file\n"); } //[DEBUG]
	char tempData[25]; //String where lines of text in the file will be stored temporary

	while (!feof(inputFile)) //Read data and save it to variables
	{
		fgets(tempData, 25, inputFile);
		if (line == 0) { currentPlayer = tempData[0] - '0'; StrCopy(tempData, scoresRow); }
		if (line == 1) { numberOfPlayers = tempData[0] - '0'; ReadPlayersScores(); }
		if (line == 2) { numberOfPenguinPerPlayer = tempData[0] - '0'; }
		if (line == 3) { StrCopy(tempData, gamePhase); }
		if (line > 3) { StrCopy(tempData, map[numberOfMapRows]); numberOfMapRows++; }
		line++;
	}

	fclose(inputFile);

	//Find number of map columns
	int i = 0;
	numberOfMapColumns = -1;
	while (map[0][i] != '\0') { i++; numberOfMapColumns++; }

	//Find number of penguins on map
	int r; for (r = 0; r < numberOfMapRows; r++) 
	{
		int c; for (c = 0; c < numberOfMapColumns; c++)
		{
			if (map[r][c] != NULL && map[r][c] != ' ' && !isdigit(map[r][c])) { numberOfPlacedPenguins++; }
		}
	}
}

void ReadPlayersScores() //Read players scores from IOFile to playersScoresArray
{
	int i;
	for (i = 1; i < numberOfPlayers + 1; i++)
	{
		int a, b = 0;
		a = scoresRow[i * 3 - 1] - '0';
		b = scoresRow[i * 3] - '0';
		playersScores[i] = a * 10 + b;
	}
}

//---------------------WRITING-TO-I/O-FILE---------------------------

void WriteDataToOutputFile(char *path)
{
	FILE* outputFile = fopen(path, "w"); //Open the file for writing
	if (outputFile == NULL) { printf("Error opening data file\n"); exit(0); }
	//else { printf("Success opening file\n"); } //[DEBUG]

	int line;
	for (line = 0; line < numberOfMapRows + 4; line++)  //Read variables and save them to data file
	{
		if (line == 0)
		{
			fprintf(outputFile, "%d ", currentPlayer);
			int i; for (i = 1; i < numberOfPlayers + 1; i++)
			{
				if (playersScores[i] < 10) { fprintf(outputFile, "0"); }
				fprintf(outputFile, "%d ", playersScores[i]);
			}
			fprintf(outputFile, "\n");
		}

		if (line == 1) { fprintf(outputFile, "%d\n", numberOfPlayers); }
		if (line == 2) { fprintf(outputFile, "%d\n", numberOfPenguinPerPlayer); }
		if (line == 3) { fprintf(outputFile, "%s", gamePhase); }
		if (line > 3) { fputs(map[line - 4], outputFile); }
	}
	fclose(outputFile);
}

//---------------------READING-FROM-ITERATIONS-FILE-----------------

void ReadIterationsFile() //Assign iterations data to the variable
{
	FILE* iterationsFile = fopen("iterations.txt", "r"); //Open the file for reading
	if (iterationsFile == NULL) { printf("No iterations file found - Creating new one\n"); FILE* iterationsFile = fopen("iterations.txt", "w"); failedTurnsIterations = 0; return; } //Check if the file has been opened
	//else { printf("Success opening file\n"); } //[DEBUG]
	char tempData[25]; //String where lines of text in the file will be stored temporary
	//FILE* iterationsFile = fopen("iterations.txt", "w"); failedTurnsIterations = 0;
	while (!feof(iterationsFile)) //Read data and save it to variable
	{
		fgets(tempData, 25, iterationsFile);
		failedTurnsIterations = atoi(tempData);
	}
	fclose(iterationsFile);
}

//---------------------WRITING-TO-ITERATIONS-FILE-------------------

void WriteIterationsFile()
{
	FILE* iterationsFile = fopen("iterations.txt", "w"); //Open the file for writing
	if (iterationsFile == NULL) { printf("Error opening iterations file\n"); exit(0); }
	//else { printf("Success opening file\n"); } //[DEBUG]
	fprintf(iterationsFile, "%d ", failedTurnsIterations);	
	fclose(iterationsFile);
}


//------------------------OTHER-FUNCTIONS---------------------------
void StrCopy(char* str_1, char* str_2) //String copy
{
	while (*str_1 != '\0')
	{
		*str_2 = *str_1++;
		++str_2;
	}
}

void PrintDataFile() //For debugging
{
	printf("\n######################\n\n");
	printf("Current player: %d\n", currentPlayer);
	printf("Players scores: ");
	int i; for (i = 1; i < numberOfPlayers + 1; i++) { printf("%d ", playersScores[i]); }
	printf("\nNumber of players: %d\n", numberOfPlayers);
	printf("Number of penguins per player: %d\n", numberOfPenguinPerPlayer);
	printf("Game phase: %s", gamePhase);
	int j; for (j  = 0; j < numberOfMapRows; j++) { printf("%s", map[j]); }

	printf("\nPenguins on map: %d\n", numberOfPlacedPenguins);
	printf("Map rows: %d\n", numberOfMapRows);
	printf("Map columns: %d\n", numberOfMapColumns);
	printf("Failed turn iterations: %d\n", failedTurnsIterations);
	printf("\n######################\n");
}