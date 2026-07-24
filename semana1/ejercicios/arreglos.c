
#include <stdio.h>

int main() {
	float onda[5] = {0.0, 1.5, 3.2, 2.1, 0.8};
	int i;

	printf("Amplitudes de onda sismica:\n");
	for (i = 0; i<5; i++) {
		printf("Punto %d: %.1f\n", i, onda[i]);
	}

	return 0;

}
