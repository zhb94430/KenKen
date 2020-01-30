#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "z3++.h"

void SolveWithZ3();
void SolveWithMiniSAT();

void ParseInput();

struct Slot
{
	int x;
	int y;
}

struct Cage
{
	int cageTarget;
	std::string cageOperator;
	std::vector<Slot> slotList;
}

std::vector<Cage> cageList;
std::string inputAddress;

int main(int argc, char const *argv[])
{
	// Process input
	if (argc != 2)
	{
		printf("Usage: KenKen [Input File]\n");

		return 0;
	}

	inputAddress = argv[1];

	ParseInput();

	SolveWithZ3();

	SolveWithMiniSAT();

	return 0;
}


void SolveWithZ3()
{
	z3::context c;
	z3::solver s(c);

	// Encode the cages
	for (int i = 0; i < cageList.size(); ++i)
	{
		
	}

	// Run the solver

	// Process the result
}

void SolveWithMiniSAT()
{
	GenerateMiniSATInput();
	//Solve with minisat
	system("./minisat ./minisatinput.txt ./minisatoutput.txt");
	std::string outputAddress = "./minisatoutput.txt";
	ProcessMiniSATOutput(outputAddress);
}

void ParseInput()
{
	FILE* inputFP = fopen(input);
	
	while (inputFP != eof)
	{
		// Parse each line into a Cage
		currentLine = ?;

		Cage currentCage;

		currentCage.cageTarget = currentLine[0];
		currentCage.cageOperator = currentLine[1];

		for (int i = 0; i < currentLine.size(); i=i+2)
		{
			Slot currentSlot;

			currentSlot.x = atoi(currentLine[i]);
			currentSlot.y = atio(currentLine[i+1]);

			currentCage.slotList.push_back(currentSlot);
		}
	}
}