#include "cudaStepper.cuh"
#include <stdio.h>
__global__ void stepper(
	float* d_firingRate,
	float* d_newFiringRate,
	float* d_connMatrix,
	int* d_sampleNeuronIndexes,
	float* d_biasVec,
	float* d_samples,
	float* stepSize,
	int* numNeurons) {

	int neurNum = blockIdx.x;
	float fireSum = 0;

	int index;
	for (int i = 0; i < (*numNeurons); i++) {
		index = neurNum * (*numNeurons) + i;
		fireSum += d_firingRate[i] * d_connMatrix[index];
	}
	fireSum += d_biasVec[neurNum];
	float fVal = 1 / (1 + exp(-fireSum));
	fVal -= d_firingRate[neurNum];
	d_newFiringRate[neurNum] = d_firingRate[neurNum] + (fVal * (*stepSize));
	index = d_sampleNeuronIndexes[neurNum];
	if (index > -1) {
		//printf("firingRate sample: %f\n", d_newFiringRate[neurNum]);
		d_samples[index] = d_newFiringRate[neurNum];
	}

}


namespace NNet {
	float** stepSys(
		int numSteps,
		int numNeurons,
		float** connMatrix,
		float* biasVec,
		float* startRate,
		std::vector<int> sampleNeurons,
		float stepSize
	) {
		//Set up flag array for sampled neurons
		int* d_sampleNeuronIndexes;
		int* sampleNeuronFlags = new int[numNeurons];
		for (int i = 0; i < numNeurons; i++) {
			sampleNeuronFlags[i] = -1;
		}
		float* sampleLayer = new float[sampleNeurons.size()];
		float** sampleRates = new float* [numSteps + 1];
		int acc = 0;
		for (int i = 0; i < sampleNeurons.size(); i++) {
			sampleNeuronFlags[sampleNeurons[i]] = acc;
			acc++;
			sampleLayer[i] = startRate[sampleNeurons[i]];
		}
		sampleRates[0] = sampleLayer;
		int size = sizeof(int) * numNeurons;
		cudaMalloc((void**)&d_sampleNeuronIndexes, size);
		cudaMemcpy(d_sampleNeuronIndexes, sampleNeuronFlags, size, cudaMemcpyHostToDevice);

		float* d_samples;
		size = sampleNeurons.size() * sizeof(float);
		cudaMalloc((void**)&d_samples, size);
		
		//Copying to GPU system properties
		float* d_biasVec; float* d_firingRate;
		float* connMatrix1D; float* d_connMatrix;
		float* d_newFiringRate;

		int index;
		connMatrix1D = new float[numNeurons * numNeurons];
		for (int i = 0; i < numNeurons; i++) {
			for (int j = 0; j < numNeurons; j++) {
				index = i * numNeurons + j;
				connMatrix1D[index] = connMatrix[i][j];
			}
		}
		size = sizeof(float) * numNeurons * numNeurons;
		cudaMalloc((void**)&d_connMatrix, size);
		cudaMemcpy(d_connMatrix, connMatrix1D, size, cudaMemcpyHostToDevice);

		size = sizeof(float) * numNeurons;
		cudaMalloc((void**)&d_biasVec, size); cudaMalloc((void**)&d_firingRate, size);
		cudaMemcpy(d_biasVec, biasVec, size, cudaMemcpyHostToDevice);
		cudaMemcpy(d_firingRate, startRate, size, cudaMemcpyHostToDevice);
		cudaMalloc((void**)&d_newFiringRate, size);

		size = sizeof(int);
		int* d_numNeur; cudaMalloc((void**)&d_numNeur, size);
		cudaMemcpy(d_numNeur, &numNeurons, size, cudaMemcpyHostToDevice);

		size = sizeof(float);
		float* d_stepSize; cudaMalloc((void**)&d_stepSize, size);
		cudaMemcpy(d_stepSize, &stepSize, size, cudaMemcpyHostToDevice);

		int sizeSample = sizeof(float) * sampleNeurons.size();
		int sizeUpdate = sizeof(float) * numNeurons;
		
		

		//Stepping system over input number of steps using stepper cuda kernel.
		/*for (int i = 0; i < numSteps; i++) {
			stepper<<<numNeurons, 1>>>(d_firingRate, d_newFiringRate, d_connMatrix, d_sampleNeuronIndexes,
				d_biasVec, d_samples, d_stepSize, d_numNeur);
			float* sampleLayer = new float[sampleNeurons.size()];
			cudaMemcpy(sampleLayer, d_samples, sizeSample, cudaMemcpyDeviceToHost);
			sampleRates[i+1] = sampleLayer;
			cudaMemcpy(d_firingRate, d_newFiringRate, sizeUpdate, cudaMemcpyDeviceToDevice);
		}*/

		//stepping system over input number of steps... Sin(t) bias'
		float time = 0.0;
		int sizeBias = sizeof(float) * numNeurons;
		
		for (int i = 0; i < numSteps; i++) {
			float sint = sin(time);
			for (int i = 0; i < numNeurons; i++) {
				biasVec[i] = abs(sint);
			}
			time += stepSize;
			cudaMemcpy(d_biasVec, biasVec, sizeBias, cudaMemcpyHostToDevice);
			stepper << <numNeurons, 1 >> > (d_firingRate, d_newFiringRate, d_connMatrix, d_sampleNeuronIndexes,
				d_biasVec, d_samples, d_stepSize, d_numNeur);
			float* sampleLayer = new float[sampleNeurons.size()];
			cudaMemcpy(sampleLayer, d_samples, sizeSample, cudaMemcpyDeviceToHost);
			sampleRates[i+1] = sampleLayer;
			cudaMemcpy(d_firingRate, d_newFiringRate, sizeUpdate, cudaMemcpyDeviceToDevice);
		}
		
		return sampleRates;
	}
}


