#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "networkData.h"
class networkData
{
public:
	networkData(std::string netFile);
	~networkData();

	int** stepNetwork(int stepNum, int* sampleNeurons);

private:
	int numNodes;
	float** connMatrix;
	float* biasVec;
	float* startRate;
	float tau;
};

