#include "networkData.h"
#include <cstdlib>
#include "cudaStepper.cuh"
#include <vector>
using namespace std;

networkData::networkData(string netFile) {
	try {
		ifstream inFile(netFile);
		string currLine;
		if (!inFile.is_open()) {
			throw "Failed to open file";
		}

		if (getline(inFile, currLine)) {
			this->numNodes = stoi(currLine);
			getline(inFile, currLine);
			this->tau = stoi(currLine);
		}
		else {
			throw "Failed to read file";
		}
		this->biasVec = new float[this->numNodes];
		for (int i = 0; i < this->numNodes; i++) {
			inFile >> this->biasVec[i];
			//cout << "Bias to neuron: " << i << " is: " << this->biasVec[i] << endl;
		}
		this->connMatrix = new float* [this->numNodes];

		this->startRate = new float[this->numNodes];
		for (int i = 0; i < this->numNodes; i++) {
			inFile >> this->startRate[i];
			this->connMatrix[i] = new float[this->numNodes];
		}
		for (int i = 0; i < this->numNodes; i++) {
			for (int j = 0; j < this->numNodes; j++) {
				inFile >> this->connMatrix[i][j];
				//cout << "i: " << i << " j: " << j << " Val: " << this->connMatrix[i][j]<<endl;
			}
		}

	}
	catch (const char* msg) {
		cerr << msg << endl;
	}
}
networkData::~networkData() {
	for (int i = 0; i < this->numNodes; i++) {
		delete(this->connMatrix[i]);
	}
	delete(this->connMatrix);
	delete(this->biasVec);
	delete(this->startRate);
}

float** networkData::stepNetwork(int stepNum, float stepSize, std::vector<int> sampleNeurons) {
	float** sampRates = NNet::stepSys(stepNum,4,this->connMatrix, this->biasVec, this->startRate,sampleNeurons, stepSize);
	return sampRates;
}