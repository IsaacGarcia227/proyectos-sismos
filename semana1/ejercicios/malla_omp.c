
#include <stdio.h>
#include <omp.h>

int main() {
	int i, j, t;
	int N = 5;
	float malla[5][5] = {0};
	float nueva[5][5] = {0};

	// Fuente sismica en el centro
	malla[2][2] = 10.0;

	printf("Nucleos disponibles: %d\n", omp_get_max_threads());
	printf("\nTiempo 0 - Fuente sismica:\n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%.0f ", malla[i][j]);
		}
		printf("\n");
	}

	//Propagar con OpenMP
	#pragma omp parallel for collapse(2)
	for (i = 1; i <N-1; i++) {
		for (j = 1; j < N-1; j++) {
			nueva[i][j] = 0.25 * (malla[i+1][j] + 
						malla[i-1][j] + 
						malla[i][j+1] + 
						malla[i][j-1]);

		}
	}

	printf("\nTiempo 1 - Onda propagada en paralelo:\n");
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			printf("%.1f ", nueva[i][j]);
		}
		printf("\n");
	}

	return 0;

}
