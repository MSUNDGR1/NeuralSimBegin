#include "networkData.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


void saveRes(float** data, int numNeurons, int numSteps,float stepSize, std::string fileName) {
	std::ofstream outFile;
	outFile.open(fileName);
	

	outFile << numSteps << "\n";
	outFile << stepSize << "\n";
	outFile << numNeurons << "\n";

	for (int i = 0; i < numSteps; i++) {
		for (int j = 0; j < numNeurons; j++) {
			outFile << data[i][j] << "\n";
		}
	}
	outFile.close();
}

int main() {

	networkData net("prob3.txt");
	std::vector<int> sampleNeurons = { 0,1,2,3};
	float** output = net.stepNetwork(25, 1.0, sampleNeurons);
	saveRes(output, 4, 25,1.0, "flatBiasStep1.txt");
	return 0;
}