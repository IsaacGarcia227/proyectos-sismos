#include <stdio.h>

//kernel - corre en gpu
__global__ void propagar(float *malla, float *nueva, int N) {

	int i = blockIdx.x + 1;
	int j = threadIdx.x + 1;

	if (i < N-1 && j < N-1) {
		nueva[i*N+j] = 0.25 *  (malla[(i+1) *N+j] + 
				    malla[(i-1)*N+j] +
				    malla[i*N+(j+1)] +
                                    malla[i*N+(j-1)]);



	}

}

int main () {
	int N = 9;
	int size = N * N * sizeof(float);

	//memoria en CPU
	float malla[81] = {0};
	float nueva[81] = {0};

	//fuente sismica en el centro
	malla[4*N+4] = 100.0;

	//memoria en GPU
	float *d_malla, *d_nueva;
	cudaMalloc(&d_malla,size);
	cudaMalloc(&d_nueva, size);

	//Copiar CPU -> GPU
	cudaMemcpy(d_malla, malla, size,cudaMemcpyHostToDevice);
	cudaMemcpy(d_nueva, nueva, size, cudaMemcpyHostToDevice);

	//Lanzar Kernel
	propagar<<<N-2, N-2>>>(d_malla, d_nueva, N);

	//copiar GPU -> CPU
	cudaMemcpy(nueva, d_nueva, size, cudaMemcpyDeviceToHost);

	//Imprimir resultado
	printf("Onda propagada con CUDA: \n");
	int i, j;
	for (i = 0; i <  N; i++){
		for (j = 0; j < N; j++){
			printf("%.1f ", nueva[i*N+j]);
		}
		printf("\n");

	}

	cudaFree(d_malla);
	cudaFree(d_nueva);

	return 0;

}
