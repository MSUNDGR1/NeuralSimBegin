#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <stdlib.h>
#include <vector>

namespace NNet {
	void stepSys(
		int numSteps,
		int numNeurons,
		float** connMatrix,
		float* biasVec,
		float* startRate,
		std::vector<int> sampleNeurons,
		float** sampleRates,
		float stepSize
	);
}