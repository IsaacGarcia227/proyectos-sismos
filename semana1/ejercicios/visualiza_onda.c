
#include <stdio.h>
#include <omp.h>

int main() {
	int i, j, t;
	int N = 9;
	float malla[9][9] = {0};
	float nueva[9][9] = {0};

	// Fuente sismica en el centro
	malla[4][4] = 100.0;

	for (t = 0; t < 4; t++){
		printf("\n === Tiempo %d ===\n", t);

		//imprimir malla con los simbolos
		for (i = 0; i < N; i++) {
			for (j =0; j < N; j++) {
				if (malla[i][j] > 50) printf("*");
				else if (malla[i][j] >  20) printf("+");
				else if (malla[i][j] > 5) printf(". ");
				else printf(" ");

			}
			printf("\n");
		}


		#pragma omp parallel for collapse(2)
		for (i = 1 ; i < N - 1; i++) {
			for (j = 1; j < N -1; j++){
				nueva[i][j] = 0.25 * (malla[i+1][j] + 
						     malla[i-1][j] + 
					             malla[i][j+1] +
						     malla[i][j-1]);
			}
		}

		//copiar nueva malla
			for (i = 1; i < N-1; i++) {
				for (j = 1; j < N-1; j++) {
					malla[i][j] = nueva[i][j];
				}
			}

	}

	return 0;
}

