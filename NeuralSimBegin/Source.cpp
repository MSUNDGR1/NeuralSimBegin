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

	networkData net("prob4.txt");
	std::vector<int> sampleNeurons = { 0,1,2,3};
	float** output = net.stepNetwork(1250, 0.02, sampleNeurons);
	saveRes(output, 4, 1250, 0.02, "flatBiasStepSMALL.txt");
	return 0;
}