#include "networkData.h"


int main() {

	networkData net("prob3.txt");
	int sampNeurons[2] = { 0,1 };
	float** output = net.stepNetwork(25, sampNeurons);
	for (int i = 0; i < 25; i++) {
		std::cout << output[i][1] << std::endl;
	}
}