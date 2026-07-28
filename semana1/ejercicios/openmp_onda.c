#include <stdio.h>
#include <omp.h>

int main() {
	int i;
	int N = 10;
	float onda[10] = {0};

	onda[5] = 10.0;

	printf("Procesando con OpenMP....\n");
	printf("Nucleos disponibles: %d\n", omp_get_max_threads());

	#pragma omp parallel for
	for (i = 1; i < N-1; i++) {
		onda[i] = onda[i] * 0.5;
	printf("Nucleo %dproceso punto %d\n",
		omp_get_thread_num(), i);
	}

	return 0;

}
