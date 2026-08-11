#include <stdio.h>

__global__ void con_global(float *malla, float *nueva, int N){
	int i = blockIdx.x + 1;
	int j = threadIdx.x + 1;

	if  (i < N-1 && j < N-1) {
	// Lee directamente de memoria global - lento
	nueva[i*N+j] = 0.25 * (malla[(i+1)*N+j] +
				malla[(i-1)*N+j] +
				malla[i*N+(j+1)] +
				malla[i*N+(j-1)]);

	}
}

__global__ void con_shared(float *malla, float *nueva, int N) {
    // Memoria compartida - rapida
	__shared__ float bloque[11][11];

	int i = blockIdx.x + 1;
	int j = threadIdx.x + 1;

	//Cargar datos en shared memory
	bloque[i][j] = malla[i*N+j];
	__syncthreads();

	if (i < N-1 && j < N-1) {
	//Lee de shared memory - rapido
		nueva[i*N+j] = 0.25 * (bloque[i+1][j] +
					bloque[i-1][j] +
					bloque[i][j+1] +
					bloque[i][j-1]);

	}
}

int main() {
	int N = 9;
	int size = N * N * sizeof(float);
	float malla[81] = {0};
	float nueva[81] = {0};

	malla[4*N+4] = 100.0;
	float *d_malla, *d_nueva;
	cudaMalloc(&d_malla, size);
	cudaMalloc(&d_nueva, size);

	cudaMemcpy(d_malla, malla, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_nueva, nueva, size, cudaMemcpyHostToDevice);


	//Probar con shared memory
	con_shared<<<N-2, N-2>>> (d_malla, d_nueva, N);

	cudaMemcpy(nueva, d_nueva, size, cudaMemcpyDeviceToHost);

	printf("Resultadoi con shared memory:  \n");
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
		printf("%.1f ", nueva[i*N+j]);
		}
		printf("\n ");
	}

	cudaFree(d_malla);
	cudaFree(d_nueva);
	return 0;
}
