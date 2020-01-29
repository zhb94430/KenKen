//The n-queens problem is amenable to encoding into SAT. Your task is to write a program that takes an integer n as input and finds a solution to the n-queens problem by encoding it into SAT.
// Program input: An integer n > 0 entered as a command line argument to your binary or script.
// Program output: A solution to the n-queens problem. (Note that I am looking for one solution and not all of them.) The output should be in the following format (example given for n = 4):

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

//Process current row/column/diag
void ProcessCurrentArray(std::vector<int>* currentArray, FILE* fp);

std::string GenerateMiniSATInput();
void ProcessMiniSATOutput(std::string outputAddress);

int termCount = 0;
int numberOfQueens = 0;

int main(int argc, char const *argv[])
{
	// Process input
	if (argc != 2)
	{
		printf("Usage: nQueenSAT [number of queens 💅]\n");

		return 0;
	}

	numberOfQueens = atoi(argv[1]);

	// Convert input into minisat input
	std::string inputAddress = GenerateMiniSATInput();

	//Solve with minisat
	system("./minisat ./minisatinput.txt ./minisatoutput.txt");

	std::string outputAddress = "./minisatoutput.txt";

	//Output result to command line
	ProcessMiniSATOutput(outputAddress);

	return 0;
}

void ProcessCurrentArray(std::vector<int>* currentArray, FILE* fp)
{
	//Simple Diag Check
	//Only exec if not diagnal arrays
	if ((currentArray->size() > 1) && ((currentArray->at(0) - currentArray->at(1)) == -1))
	{
		std::string currentLine;

		for (int i = 0; i < currentArray->size(); ++i)
		{
			currentLine += std::to_string(currentArray->at(i));
			currentLine += " ";
		}

		currentLine += "0";

		fprintf(fp, "%s\n", currentLine.c_str());
		termCount++;
	}

	// Exclusive ors
	for (int i = 0; i < currentArray->size(); ++i)
	{
		for (int j = i+1; j < currentArray->size(); ++j)
		{
			std::string currentLine;

			currentLine += "-";
			currentLine += std::to_string(currentArray->at(i));
			currentLine += " -";
			currentLine += std::to_string(currentArray->at(j));
			currentLine += " 0";

			fprintf(fp, "%s\n", currentLine.c_str());
			termCount++;
		}
	}
}

std::string GenerateMiniSATInput()
{
	std::string inputAddress = "./input.txt";
	FILE* inputFP;
	inputFP = fopen(inputAddress.c_str(), "w+");

	if (inputFP == NULL)
	{
		printf("Fail to create minisat input file\n");

		return NULL;
	}
	// Core of the assignment
	else
	{
		// 1  2  3  4  5
		// 6  7  8  9  10
		// 11 12 13 14 15
		// 16 17 18 19 20
		// 21 22 23 24 25

		// Rows
		for (int i = 0; i < numberOfQueens; ++i) 
		{
			std::vector<int> currentRow;

			for(int j = 0; j < numberOfQueens; ++j) 
			{

				currentRow.push_back(i * numberOfQueens + j + 1); // Plus one to offset
			}

			ProcessCurrentArray(&currentRow, inputFP);
		}

		// Columns
		for (int i = 0; i < numberOfQueens; ++i) 
		{
			std::vector<int> currentCol;

			for(int j = 0; j < numberOfQueens; ++j) 
			{

				currentCol.push_back(j * numberOfQueens + i + 1); // Plus one to offset
			}

			ProcessCurrentArray(&currentCol, inputFP);
		}

		// Diag
		// https://stackoverflow.com/questions/20420065/loop-diagonally-through-two-dimensional-array
		for (int k = 0; k < numberOfQueens * 2; ++k) 
		{
			std::vector<int> currentDiag;

			for (int j = 0; j <= k; ++j) 
			{
				int i = k - j;

				if (i < numberOfQueens && j < numberOfQueens)
				{
					currentDiag.push_back(i * numberOfQueens + j + 1);
				}
			}

			ProcessCurrentArray(&currentDiag, inputFP);
		}
	}

	// Header
	FILE* headerFP = fopen("./header.txt", "w+");
	fprintf(headerFP, "p cnf %d %d\n", numberOfQueens*numberOfQueens, termCount);

	fclose(inputFP);
	fclose(headerFP);

	system("cat ./header.txt ./input.txt > ./minisatinput.txt");

	return inputAddress;
}

void ProcessMiniSATOutput(std::string outputAddress)
{
	FILE* outputFP;
	outputFP = fopen(outputAddress.c_str(), "r");

	if (outputFP == NULL)
	{
		printf("Fail to open minisat output\n");

		return;
	}

	else
	{
		// Read the output file
		fseek(outputFP, 0, SEEK_END);
		size_t outputSize = ftell(outputFP);
		char* output = new char[outputSize];
		rewind(outputFP);
		fread(output, sizeof(char), outputSize, outputFP);
		std::string outputString(output);

		// Satisfiable
		if (outputString != std::string("UNSAT\n"))
		{
			// Process the output
			std::string result = outputString.substr(4);
			std::string tempString;
			std::vector<int> convertedResult;

			for (int i = 0; i < result.size(); ++i)
			{
				if (result[i] == std::string(" ")[0])
				{
					//Append and clear temp string
					if (tempString != std::string(""))
					{
						convertedResult.push_back(atoi(tempString.c_str()));
						tempString = "";
					}
				}

				else
				{
					tempString.push_back(result[i]);
				}
			}

			// Print the output
			for (int i = 0; i < convertedResult.size(); ++i)
			{
				if (convertedResult[i] < 0)
				{
					printf("[ ]");
				}
				else
				{
					printf("[X]");
				}

				if ((i + 1) % numberOfQueens == 0 )
				{
					printf("\n");
				}
			}

		}

		// Unsatisfiable
		else
		{
			printf("n = %d is unsatisfiable\n", numberOfQueens);
		}
	}

	fclose(outputFP);
}