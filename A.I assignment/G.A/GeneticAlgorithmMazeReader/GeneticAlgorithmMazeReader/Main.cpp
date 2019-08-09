#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>     /* atoi */
#include <stdio.h>      /* printf, fgets */
#include <cstring>
#include <vector>
#include <random>
#include "ctime"
#include "cmath"
#include "ctgmath"
//SYSTEM
//boolean to be changed to false when an individual finds the finish
bool SYSrunning = true;
bool SYScontinue = true;
int SYSrunCycles = 1;
int SYSXtemp = 0;                                                            //temporary storage for the iterater when creating the maze
int SYSYtemp = 0;
clock_t SYSstartTime;
clock_t SYSendTime;


//float CHRfitnessScore[CHRcount];
float CHRgenerationFitnessTotal = { 0 };

int CHRgene = 0;
//starting and finishing point of the maze
int CHRYfinish = 0;
int CHRXfinish = 0;
int CHRYstartingPosition = 0;
int CHRXstartingPosition = 0;
//position of the individual that is to be modified to see where the ai goes in the maze
int CHRYcurrentPosition = 0;
int CHRXcurrentPosition = 0;

//Mutation rate for chromosomes, which will change based lengh of chormosomes         
float CHRmutationRate = 0; 
//MAZE
//stores the size of the maze
int MAZEXsize = 0;
int MAZEYsize = 0;
//function that assigns the mutation to be proportional to the size of the chromosome count
float CHRmutationAssigner(int _CHRlength)
{
	float SYSvalue = _CHRlength; 
	float MutationRate;
	MutationRate = 1 / SYSvalue; //With this function the mutation will always be set to a probaility of 1 mutation per chromosome
std::cout << "Current mutation chance is: " << MutationRate * 100 << "%" << std::endl;
	return MutationRate;
}

//function that removes the white space from the text file and stores the result in an output string
void splitStringWhitespace(std::string& input, std::vector<std::string>& output)
{
	std::string curr;
	output.clear();
	for (size_t i = 0; i < input.length(); i++)
	{
		if (input.at(i) == ' ' ||
			input.at(i) == '\r' ||
			input.at(i) == '\n' ||
			input.at(i) == '\t')
		{
			if (curr.length() > 0)
			{
				output.push_back(curr);
				curr = "";
			}
		}
		else
		{
			curr += input.at(i);
		}
	}
	if (curr.length() > 0)
	{
		output.push_back(curr);
	}
}
//randomly selects a number between 0 and 1
float randomizer()
{
	std::uniform_real_distribution<float> distribution(0, 1);
	std::random_device rd;
	std::default_random_engine generator(rd());
	return distribution(generator);
}

//randomly choose an individual, based on their fitness, for breeding
int IndividualSelector(std::vector<float> CIndividual, int _CHRcount)
{
	float SpinningArrow = { 0 };
	float total = 0;
	SpinningArrow = randomizer();
	//Convert to percentage
	SpinningArrow = SpinningArrow * 100;
	for (int i = 0; i < _CHRcount; i++)
	{
		if (SpinningArrow >= total && SpinningArrow < total + CIndividual[i])
		{
			std::cout << "Select Individual : " << i + 1 << " ";
			return i;
		}
		else
		{
			total = total + CIndividual[i];
		}
	}
}
//find the chromosome with the best fitness and return the chromosome number. !fitness
int BestFitness(std::vector<float> fitness, int _CHRcount)
{
	float bestFitness = { 0 };
	float chromosomeNumber = { 0 };
	for (int i = 0; i < _CHRcount; i++)
	{
		if (fitness[i] > bestFitness)                                        //if the current best chromosome is smaller that the index, store the new index in bestFitness
		{
			bestFitness = fitness[i];
			chromosomeNumber = i;
		}
	}
	std::cout << "Select Individual : " << chromosomeNumber + 1 << " ";
	return chromosomeNumber;
}


//creates a random number between 0 and 1, and then checks to see if it makes the crossover rate to determin whether a two parents will breed
bool CHRcrossOverCheck()
{
	float CrossOverRate = 0.7;
	float CrossOverScore = 0;
	CrossOverScore = randomizer();
	if (CrossOverScore <= CrossOverRate)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


//Intial Chromosome generator
void CHRGenerator(std::vector<std::vector<int>>&CHRvector, int _CHRlength, int _CHRcount)                 
{
	for (int y = 0; y < _CHRcount; y++)
	{
		for (int x = 0; x < _CHRlength; x++)
		{
			CHRvector[y][x] = randomizer();
			CHRvector[y][x] = (int)round(CHRvector[y][x]);
		}
		std::cout << std::endl << std::endl;
	}
}
//Function to provide the user with an idea of how many moves the chromosome should have based on the size of the maze being used
int CHRlengthGenerator(int _MAZEXsize, int _MAZEYsize)
{
	
	int _CHRlength= (_MAZEXsize*_MAZEYsize)-((_MAZEXsize*_MAZEYsize)/3);
	std::cout << "Minimum required moves for a maze of this size: " << _CHRlength / 2;
	_CHRlength = _MAZEXsize*_MAZEYsize;
	std::cout << " " << "Recommended amount of moves: " << _CHRlength / 2 << std::endl;
	return 0;
}

int main()
{
	
	//Access maze data and assign X Y size
	std::cout << "Hello welcome to my program of a genetic algortigm solving a maze \n";
	//To change file change the name of "myfile"
	std::ifstream myfile("Maze_4x6.txtt");
	std::string line;
	std::vector < std::string > Splitline;
	getline(myfile, line);                                                    //moving the contents of the .txt file into a string 
	splitStringWhitespace(line, Splitline);                                    //removing the blank spaces from string
	MAZEXsize = atoi(Splitline.at(0).c_str());                                //first and second characters of the string will give the size of the maze
	MAZEYsize = atoi(Splitline.at(1).c_str());                                //so they are converted into an int and stored for later
	std::cout << "the size of the currently targeted maze is: " << MAZEXsize << "," << MAZEYsize << std::endl;
	
	//Chromosome count and size creation
	std::cout << "//////////////////////////////////////////////////////////// \n ";
	std::cout << "Please select the amount of moves you would like your chromosomes to have \n";
	CHRlengthGenerator(MAZEXsize, MAZEYsize);
	int CHRlength= 0;
	std::cin >> CHRlength;
	CHRlength= CHRlength*2;        
	std::cout << "Please select the amount of chromosomes you would like to have (4 or above) \n";
	int CHRcount = 0;
	std::cin >> CHRcount;
	//Paramaters for fitness calculation
	// x and y offset from the finishing point	
	std::vector<int> CHRYoffset(CHRcount); 
	std::vector<int> CHRXoffset(CHRcount);
	std::vector <float> CHRfitness(CHRcount);  
	std::vector<float>CHRfitnessScore(CHRcount);

	//Declaring/Initialising the Chromosomoes themselves
	std::vector<std::vector<int>> CHRvector(CHRcount, std::vector<int>(CHRlength));   // the array to store the chromosomes (default all set to 0)
	std::vector <int> CHRtempVector(CHRlength);  //this keeps track of how far through the chromosome it is when stepping through the maze
	std::vector< std::vector<int> > PCHRvector(CHRcount, std::vector<int>(CHRlength));
	CHRGenerator(CHRvector, CHRlength, CHRcount);

	//Creating the Maze
	//Declaring a 20 by 20 maze in which the actual smaller maze will be on
	int MAZEarray[20][20] = { 0 };                                            //2D array that stores the information of the mazes layout
	std::cout << std::endl;
	//assigning the the .txt file information into the maze array
	for (int i = 2; i < Splitline.size(); i++)
	{
		MAZEarray[SYSXtemp][SYSYtemp] = atoi(Splitline.at(i).c_str());
		SYSXtemp++;
		//std::cout << MAZEarray[SYSXtemp][SYSYtemp] << " ";
		if (SYSXtemp >= MAZEXsize)
		{
			SYSXtemp = 0;
			SYSYtemp++;
			std::cout << std::endl;
		}
	}
	// outputting the maze into the console
	for (int y = 0; y < MAZEYsize; y++)
	{
		for (int x = 0; x < MAZEXsize; x++)
		{
			std::cout << MAZEarray[x][y] << " ";
		}
		std::cout << std::endl;
	}
	//assigning starting point
	for (int y = 0; y < MAZEYsize; y++)
	{
		for (int x = 0; x < MAZEXsize; x++)
		{
			if (MAZEarray[x][y] == 2)
			{
				CHRYstartingPosition = y;
				CHRXstartingPosition = x;
			}
		}
	}
	std::cout << std::endl;
	//assinging finish point
	for (int y = 0; y < MAZEYsize; y++)
	{
		for (int x = 0; x < MAZEXsize; x++)
		{
			if (MAZEarray[x][y] == 3)
			{
				CHRYfinish = y;
				CHRXfinish = x;
			}
		}
	}
	std::cout << std::endl;
	//Assiging Start Position of Chromosomes to start position of the maze
	CHRYcurrentPosition = CHRYstartingPosition;
	CHRXcurrentPosition = CHRXstartingPosition;

	//Start Timer to detmine how long the algorithm will take to solve the maze
	SYSstartTime = clock();
	while (SYSrunning)
	{
		std::cout << "THIS IS GENERATION : " << SYSrunCycles << std::endl;
		for (int y = 0; y < CHRcount; y++)
		{
			for (int x = 0; x < CHRlength; x++)
			{
				std::cout << CHRvector[y][x];
				if (x % 2)
				{
					std::cout << " ";
				}
			}
			std::cout << std::endl << std::endl;
		}
		for (int i = 0; i < CHRcount; i++)
		{
			while (CHRgene < CHRlength)
			{
				if (CHRvector[i][CHRgene] == 1 && CHRvector[i][CHRgene + 1] == 1)
					//move up;
				{
					CHRYcurrentPosition = CHRYcurrentPosition - 1;
					if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
					{
						CHRYcurrentPosition = CHRYcurrentPosition + 1;
					}
				}
				else if (CHRvector[i][CHRgene] == 0 && CHRvector[i][CHRgene + 1] == 1)
					//move right
				{
					CHRXcurrentPosition = CHRXcurrentPosition + 1;
					if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
					{
						CHRXcurrentPosition = CHRXcurrentPosition - 1;
					}
				}
				else if (CHRvector[i][CHRgene] == 0 && CHRvector[i][CHRgene + 1] == 0)
					//move down
				{
					CHRYcurrentPosition = CHRYcurrentPosition + 1;
					if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
					{
						CHRYcurrentPosition = CHRYcurrentPosition - 1;
					}
				}
				else if (CHRvector[i][CHRgene] == 1 && CHRvector[i][CHRgene + 1] == 0)
					//move left
				{
					CHRXcurrentPosition = CHRXcurrentPosition - 1;
					if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
					{
						CHRXcurrentPosition = CHRXcurrentPosition + 1;
					}
				}
				if (CHRYcurrentPosition == CHRYfinish && CHRXcurrentPosition == CHRXfinish)
				{
					std::cout << CHRXcurrentPosition << CHRYcurrentPosition << "                                   GOAL REACHED";
					for (int z = 0; z < CHRlength; z++)
					{
						CHRtempVector[z] = CHRvector[i][z];
						std::cout << CHRtempVector[z];
					}
					SYSendTime = clock();
					SYSrunning = false;
					SYScontinue = false;
					break;
				}
				CHRgene = CHRgene + 2;
				std::cout << CHRXcurrentPosition;
				std::cout << CHRYcurrentPosition;
				std::cout << " ";
			}
			std::cout << std::endl;
			std::cout << "^ coordinates of each step taken from chromosome : " << i + 1 << std::endl;
			CHRgene = 0;
			//Calculate the Fitness using the offset the last position of the choromosome and the finish position of the maze
			CHRXoffset[i] = CHRXfinish - CHRXcurrentPosition;
			CHRYoffset[i] = CHRYfinish - CHRYcurrentPosition;
			CHRfitness[i] = CHRXoffset[i] + CHRYoffset[i] + 1;
			CHRfitness[i] = 1 / CHRfitness[i];
			
			std::cout << "Fitness of Chromosome: ";
			std::cout << CHRfitness[i] << std::endl << std::endl;
			
			CHRgenerationFitnessTotal += CHRfitness[i];
			
			CHRXcurrentPosition = CHRXstartingPosition;
			CHRYcurrentPosition = CHRYstartingPosition;

		}
		if (SYScontinue)
		{
			//Create the Individuals
			for (int i = 0; i < CHRcount; i++)
			{
				CHRfitnessScore[i] = CHRfitness[i] / CHRgenerationFitnessTotal;
				CHRfitnessScore[i] = CHRfitnessScore[i] * 100;
				//std::cout << "This is individual: " << CHRfitnessScore[i] << std::endl;
			}
			CHRgenerationFitnessTotal = 0;
			std::cout << std::endl;
			//Generator for Spinning Arrow Function
			int i = 0;
			int z = 0;

			//Elite parent that will never digress
			std::cout << "This is Parent 1: ";
			z = BestFitness(CHRfitness, CHRcount);
			for (int x = 0; x < CHRlength; x++)
			{
				PCHRvector[0][x] = CHRvector[z][x];
				std::cout << PCHRvector[0][x];
			}
			std::cout << std::endl;

			std::cout << std::endl;
			//Rest of parents
			for (int y = 1; y < CHRcount; y++)
			{
				std::cout << "This is Parent " << y + 1 << ": ";
				i = IndividualSelector(CHRfitnessScore, CHRcount);
				for (int x = 0; x < CHRlength; x++)
				{
					PCHRvector[y][x] = CHRvector[i][x];
					std::cout << PCHRvector[y][x];
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
			std::cout << "Parents will crossover check: ";
			int a = { 2 };
			int b = { 0 };
			b = a + 1;
			for (a; a < CHRcount; a++)
			{
				if (CHRcrossOverCheck() == 1)
				{
					std::cout << "success!" << std::endl;
					for (int x = 0; x < CHRlength; x++)
					{
						if (x > CHRlength / 2)
						{
							CHRtempVector[x] = PCHRvector[a][x];
						}
					}
					for (int x = 0; x < CHRlength; x++)
					{
						if (x > CHRlength / 2)
						{
							PCHRvector[a][x] = PCHRvector[b][x];
						}
					}
					for (int x = 0; x < CHRlength; x++)
					{
						if (x > CHRlength / 2)
						{
							PCHRvector[b][x] = CHRtempVector[x];
						}
					}
				}
				else
				{
					std::cout << "failed" << std::endl;
				}
			}
			for (int y = 0; y < CHRcount; y++)
			{
				for (int x = 0; x < CHRlength; x++)
				{
					CHRvector[y][x] = PCHRvector[y][x];
				}
			}
			//this is where we bring in the x-men ;)
			CHRmutationRate= CHRmutationAssigner(CHRlength);
			float randomChance;
			std::cout << "Error Time" << CHRmutationRate << "\n";
			for (int y = 2; y < CHRcount; y++)
			{
				for (int x = 0; x < CHRlength; x++)
				{
					randomChance = randomizer();
					if (randomChance < CHRmutationRate)
					{
						std::cout << "mutation has happened" << randomChance << std::endl;
						if (CHRvector[y][x] == 1)
						{
							CHRvector[y][x] = 0;
						}
						else
						{
							CHRvector[y][x] = 1;
						}
					}
				}
			}
			SYSrunCycles++;
			std::cout << std::endl << std::endl;
		}
	}




	while (CHRgene < CHRlength)
	{
		if (CHRtempVector[CHRgene] == 1 && CHRtempVector[CHRgene + 1] == 1)
			//move up;
		{
			CHRYcurrentPosition = CHRYcurrentPosition - 1;
			if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
			{
				CHRYcurrentPosition = CHRYcurrentPosition + 1;
			}
			else
			{
				MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] = 4;
			}
		}
		else if (CHRtempVector[CHRgene] == 0 && CHRtempVector[CHRgene + 1] == 1)
			//move right
		{
			CHRXcurrentPosition = CHRXcurrentPosition + 1;
			if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
			{
				CHRXcurrentPosition = CHRXcurrentPosition - 1;
			}
			else
			{

				MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] = 4;

			}
		}
		else if (CHRtempVector[CHRgene] == 0 && CHRtempVector[CHRgene + 1] == 0)
			//move down
		{
			CHRYcurrentPosition = CHRYcurrentPosition + 1;
			if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
			{
				CHRYcurrentPosition = CHRYcurrentPosition - 1;
			}
			else
			{

				MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] = 4;

			}
		}
		else if (CHRtempVector[CHRgene] == 1 && CHRtempVector[CHRgene + 1] == 0)
			//move left
		{
			CHRXcurrentPosition = CHRXcurrentPosition - 1;
			if (MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] == 1 || CHRXcurrentPosition < 0 || CHRYcurrentPosition < 0 || CHRXcurrentPosition >= MAZEXsize || CHRYcurrentPosition >= MAZEYsize)
			{
				CHRXcurrentPosition = CHRXcurrentPosition + 1;
			}
			else
			{

				MAZEarray[CHRXcurrentPosition][CHRYcurrentPosition] = 4;

			}

		}
		CHRgene = CHRgene + 2;
		std::cout << CHRXcurrentPosition;
		std::cout << CHRYcurrentPosition;
		std::cout << " ";
	}
	double time_elapsed = double(SYSendTime - SYSstartTime);
	std::cout << std::endl;
	std::cout << "Time to calculate the route (ms): " << time_elapsed << std::endl;

	MAZEarray[CHRXfinish][CHRYfinish] = 3;
	MAZEarray[CHRXstartingPosition][CHRYstartingPosition] = 2;
	std::cout << std::endl;
	for (int y = 0; y < MAZEYsize; y++)
	{
		for (int x = 0; x < MAZEXsize; x++)
		{
			if (MAZEarray[x][y] == 0)
				std::cout << "0";
			else if (MAZEarray[x][y] == 1)
				std::cout << "1"; //Wall
			else if (MAZEarray[x][y] == 2)
				std::cout << "2"; //start
			else if (MAZEarray[x][y] == 3)
				std::cout << "3"; //finish
			else if (MAZEarray[x][y] == 4)
				std::cout << "R"; //route

		}
		std::cout << std::endl;
	}
	//Destoy the maze file
	myfile.close();
	std::cin.get();
	std::cin.get();

	return 0;
}


