#include <stdio.h>
#include <omp.h>

int main() {
	printf("Programa sin paralelismo:\n");
	int  i;
	for (i = 0; i< 8; i++) {
		printf("Tarea%d ejecutada en secuencia\n", i);
	}

	printf("\n Programa CON paralelismo:\n");
	#pragma omp parallel for
	for (i = 0; i < 8; i++) {
		printf("Tarea %d ejecutada por nucleo %d\n",
			i, omp_get_thread_num());
	}

	return 0;
	
}



