
/*EL PROGRAMA CORRE UN CODIGO  DE MANERA 
NORMAL Y LA OTRA PARTE CON PARALELISMO CONDIRECTRICIES DE OPEN ACC*/

#include <stdio.h>

int main() {
	int i;
	int tareas[8] = {0};

	printf("Con OpenACC en GPU:\n");

	#pragma acc parallel loop
	for (i = 0; i < 8; i++) {
		tareas[i] = i * 2;
	}

	for (i = 0; i < 8; i++) {
		printf("Tarea %d resultado: %d\n", i, tareas[i]);
	}

	return 0;

}






