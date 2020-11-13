#include "networkData.h"
#include <vector>

int main() {

	networkData net("prob3.txt");
	std::vector<int> sampleNeurons = { 0,1,2,3 };
	float** output = net.stepNetwork(25, sampleNeurons);
	for (int i = 0; i < 25; i++) {
		std::cout << "Neuron 0: "<<output[i][0] << std::endl;
		std::cout << "Neuron 1: " << output[i][1] << std::endl;
		std::cout << "Neuron 2: " << output[i][2] << std::endl;
	}
}