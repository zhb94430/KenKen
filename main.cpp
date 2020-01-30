#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "z3++.h"

void SolveWithZ3(std::string input);


int main(int argc, char const *argv[])
{
	// Process input
	if (argc != 2)
	{
		printf("Usage: KenKen [Input File]\n");

		return 0;
	}

	std::string inputAddress = argv[1];

	// Convert input into Z3 format
	SolveWithZ3(inputAddress)

	GenerateMiniSATInput();
	//Solve with minisat
	system("./minisat ./minisatinput.txt ./minisatoutput.txt");
	std::string outputAddress = "./minisatoutput.txt";
	ProcessMiniSATOutput(outputAddress);


	return 0;
}


void SolveWithZ3(std::string input)
{
	FILE* inputFP = fopen(input);

	// Parse the input
	

	// Encode the input

	// Run the solver

	// Process the result
}