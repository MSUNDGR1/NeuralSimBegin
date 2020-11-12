#include "networkData.h"
#include <cstdlib>

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

int** networkData::stepNetwork(int stepNum, int* sampleNeurons) {

}