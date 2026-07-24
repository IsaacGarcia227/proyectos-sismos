
#include <stdio.h>

int main() {
	int filas = 3, columnas = 3;
	float malla[3][3] = {
		{0.0, 1.0, 0.0},
		{1.0, 5.0, 1.0},
		{0.0, 1.0, 0.0},
	};
	int i, j;

	printf("Malla sismica de 3x3:\n");
	for (i = 0; i < filas; i++) {
		for (j = 0; j < columnas; j++) {
			printf("%.1f ", malla[i][j]);
		
		}
		printf("\n");

	}

	
	return 0;
}
	
