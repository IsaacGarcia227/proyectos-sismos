#include <stdio.h>

int main() {
	float magnitud;
	int profundidad;
	char ubicacion[50];

	printf("Ingresa la magnitud: ");
	scanf("%f", &magnitud);

	printf("Ingresa la profundidad (km): ");
	scanf("%d", &profundidad);

	printf("Ingresa la ubicacion: ");
	scanf("%s", ubicacion);

	printf("\nSismo detectado!\n");
	printf("Magnitud: %.1f\n", magnitud);
	printf("Profundidad: %d km\n", profundidad);
	printf("Ubicacion: %s\n", ubicacion);
	
	return 0;
}

