#include <stdio.h>

int main() {
	float magnitud;

	printf("Ingresa la magnitud del sismo: ");
	scanf("%f", &magnitud);

	if (magnitud >= 7.0) {
		printf("ALERTA: Sismo mayor, magnitud %.1f\n", magnitud);
	} else if (magnitud >= 5.0) {
		printf("Sismo moderado, magnitud %.1f\n", magnitud);
	} else {
		printf("Sismo menor, magnitud %.1f\n", magnitud);
	}

	return 0;
}
