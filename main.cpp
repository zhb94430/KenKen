#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "z3++.h"

void SolveWithZ3();
void SolveWithMiniSAT();
void ParseInput();
void tokenize(std::string const &str, const char delim,
			std::vector<std::string> &out);
std::string getVarName(int x, int y);
int Two2One(int x, int y);

struct Slot
{
	int x;
	int y;
	std::string varName;
};

struct Cage
{
	int cageTarget;
	std::string cageOperator;
	std::vector<Slot> slotList;
};

// Variables
std::vector<Cage> cageList;
std::string inputAddress;
int axisCount;

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

	// SolveWithMiniSAT();

	return 0;
}


void SolveWithZ3()
{
	z3::context c;

	// Instantiate all the int_const
	z3::expr_vector constList(c);
	for (int i = 0; i < axisCount; ++i)
	{
		for (int j = 0; j < axisCount; ++j)
		{
			constList.push_back(c.int_const(getVarName(i, j).c_str()));
		}
	}

	z3::solver s(c);







	// Constraint for digits
	for (int i = 0; i < axisCount; ++i)
	{
		for (int j = 0; j < axisCount; ++j)
		{
			s.add(constList[Two2One(i,j)] >= 1 && constList[Two2One(i,j)] <= axisCount);
		}
	}

	// Constraint for rows & columns
	for (int i = 0; i < axisCount; ++i)
	{
		z3::expr_vector temp(c);
		
		for (int j = 0; j < axisCount; ++j)
		{
			temp.push_back(constList[Two2One(i, j)]);
		}

		s.add(distinct(temp));
	}

	for (int j = 0; j < axisCount; ++j)
	{
		z3::expr_vector temp(c);
		
		for (int i = 0; i < axisCount; ++i)
		{
			temp.push_back(constList[Two2One(i, j)]);
		}

		s.add(distinct(temp));
	}

	// Constraint for all the cages
	for (int i = 0; i < cageList.size(); ++i)
	{
		Cage* currentCage = &cageList[i];
		
		z3::expr target = c.int_val(currentCage->cageTarget);
		

		// Fetch the operator and target, then construct
		if (currentCage->cageOperator == "+")
		{
			z3::expr expression = c.int_val(0);

			for (int j = 0; j < currentCage->slotList.size(); ++j)
			{
				Slot* currentSlot = &currentCage->slotList[j];

				expression = expression + constList[Two2One(currentSlot->x,currentSlot->y)];
			}

			s.add(target == expression);
		}

		else if (currentCage->cageOperator == "-")
		{
			z3::expr expression = c.bool_val(false);


			for (int j = 0; j < currentCage->slotList.size(); ++j)
			{
				Slot* currentSlot = &currentCage->slotList[j];

				z3::expr subExpression = constList[Two2One(currentSlot->x,currentSlot->y)];

				// Before j
				for (int k = j-1; k >= 0; --k)
				{
					Slot* currentSlot2 = &currentCage->slotList[k];

					subExpression = subExpression - constList[Two2One(currentSlot2->x,currentSlot2->y)];
				}

				// After j
				for (int k = j+1; k < currentCage->slotList.size(); ++k)
				{
					Slot* currentSlot2 = &currentCage->slotList[k];

					subExpression = subExpression - constList[Two2One(currentSlot2->x,currentSlot2->y)];
				}

				subExpression = (subExpression == target);

				expression = expression || subExpression;
			}

			s.add(expression);
		}

		else if (currentCage->cageOperator == "*")
		{
			z3::expr expression = c.int_val(1);

			for (int j = 0; j < currentCage->slotList.size(); ++j)
			{
				Slot* currentSlot = &currentCage->slotList[j];

				expression = expression * constList[Two2One(currentSlot->x,currentSlot->y)];
			}

			s.add(target == expression);
		}

		else if (currentCage->cageOperator == "/")
		{
			z3::expr expression = c.bool_val(false);

			for (int j = 0; j < currentCage->slotList.size(); ++j)
			{
				Slot* currentSlot = &currentCage->slotList[j];

				z3::expr subExpression = constList[Two2One(currentSlot->x,currentSlot->y)];

				// Before j
				for (int k = j-1; k >= 0; --k)
				{
					Slot* currentSlot2 = &currentCage->slotList[k];

					subExpression = subExpression / constList[Two2One(currentSlot2->x,currentSlot2->y)];
				}

				// After j
				for (int k = j+1; k < currentCage->slotList.size(); ++k)
				{
					Slot* currentSlot2 = &currentCage->slotList[k];

					subExpression = subExpression / constList[Two2One(currentSlot2->x,currentSlot2->y)];
				}

				subExpression = (subExpression == target);

				expression = expression || subExpression;
			}

			s.add(expression);
		}

		else if (currentCage->cageOperator == "g")
		{
			z3::expr expression = c.int_val(0);

			for (int j = 0; j < currentCage->slotList.size(); ++j)
			{
				Slot* currentSlot = &currentCage->slotList[j];

				expression = constList[Two2One(currentSlot->x,currentSlot->y)];
			}

			s.add(target == expression);
		}

		else
		{
			printf("Operator not valid\n");
		}

		
	}







	// Run the solver
	switch (s.check()) {
    case z3::unsat:   std::cout << "unsat\n"; break;
    case z3::sat:     std::cout << "sat\n"; break;
    case z3::unknown: std::cout << "unknown\n"; break;
    }

	// Process the result
	z3::model m = s.get_model();
	std::stringstream outputStringStream;
	std::vector<std::string> outputString;
	std::vector<std::vector<int>> outputMatrix(axisCount, std::vector<int>(axisCount, -1));

    // traversing the model
    for (unsigned i = 0; i < m.size(); i++) 
    {
        z3::func_decl v = m[i];
        // this problem contains only constants
        // assert(v.arity() == 0); 
        outputStringStream << v.name() << " " << m.get_const_interp(v) << " ";
    }

    std::string outputStringRaw = outputStringStream.str();

    tokenize(outputStringRaw, ' ', outputString);

    for (int i = 0; i < axisCount*axisCount*2; i+=2)
    {
    	std::string identifier = outputString[i];
    	std::string value = outputString[i+1];
    	std::string xCoord;
    	std::string yCoord;

    	if (identifier.size() == 3)
    	{
    		xCoord = identifier[1];
    		yCoord = identifier[2];
    	}
    	else
    	{
    		int xIndex = 1;
    		int yIndex = identifier.size() / 2 + 1;
    		int indexSize = identifier.size() / 2;

    		xCoord = identifier.substr(xIndex, xIndex+indexSize-1);
    		yCoord = identifier.substr(yIndex);
    	}

    	outputMatrix[std::stoi(xCoord)][std::stoi(yCoord)] = std::stoi(value);
    }

    for (int i = 0; i < axisCount; ++i)
    {
    	for (int j = 0; j < axisCount; ++j)
    	{
    		printf("%d ", outputMatrix[i][j]);
    	}

    	printf("\n");
    }
}

// void SolveWithMiniSAT()
// {
// 	GenerateMiniSATInput();
// 	//Solve with minisat
// 	system("./minisat ./minisatinput.txt ./minisatoutput.txt");
// 	std::string outputAddress = "./minisatoutput.txt";
// 	ProcessMiniSATOutput(outputAddress);
// }

void ParseInput()
{
	std::ifstream inputStream(inputAddress.c_str());
	
	if (!inputStream)
	{
		std::cerr << "Failed to open input file" << std::endl;
	}

	std::string currentLineRaw;

	// Get the first line, N?
	std::getline(inputStream, currentLineRaw);

	axisCount = atoi(currentLineRaw.c_str());

	while (std::getline(inputStream, currentLineRaw))
	{
		// Parse each line into a Cage
		Cage currentCage;
		std::vector<std::string> currentLine;

		tokenize(currentLineRaw, ' ', currentLine);

		currentCage.cageTarget = atoi(currentLine[0].c_str());
		currentCage.cageOperator = currentLine[1];

		for (int i = 2; i < currentLine.size(); i=i+2)
		{
			Slot currentSlot;

			currentSlot.x = atoi(currentLine[i].c_str());
			currentSlot.y = atoi(currentLine[i+1].c_str());

			currentSlot.varName = getVarName(currentSlot.x, currentSlot.y);

			currentCage.slotList.push_back(currentSlot);
		}

		cageList.push_back(currentCage);
	}
}


// https://www.techiedelight.com/split-string-cpp-using-delimiter/
void tokenize(std::string const &str, const char delim,
			std::vector<std::string> &out)
{
	// construct a stream from the string
	std::stringstream ss(str);

	std::string s;
	while (std::getline(ss, s, delim)) {
		out.push_back(s);
	}
}

std::string getVarName(int x, int y)
{
	return "v" + std::to_string(x) + std::to_string(y);
}

int Two2One(int x, int y)
{
	return x * axisCount + y;
}