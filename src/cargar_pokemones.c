#include "cargar_pokemones.h"

enum TIPO definir_tipo(char texto)
{
	enum TIPO tipo = -1;

	if (texto == 'N') {
		tipo = NORMAL;
	} else if (texto == 'F') {
		tipo = FUEGO;
	} else if (texto == 'A') {
		tipo = AGUA;
	} else if (texto == 'P') {
		tipo = PLANTA;
	} else if (texto == 'E') {
		tipo = ELECTRICO;
	} else if (texto == 'R') {
		tipo = ROCA;
	}

	return tipo;
}

struct ataque *cargar_ataque_pokemon(char linea[MAX_LINEA])
{
	struct ataque *nuevo_ataque = malloc(sizeof(struct ataque));

	if (nuevo_ataque == NULL)
		return NULL;

	char tipo;
	char valor;

	int cantidad = sscanf(linea, "%[^;];%c;%c", nuevo_ataque->nombre, &tipo,
			      &valor);

	if (cantidad != 3) {
		free(nuevo_ataque);
		return NULL;
	}

	if (definir_tipo(tipo) == -1) {
		free(nuevo_ataque);
		return NULL;
	}

	nuevo_ataque->poder = (unsigned int)atoi(&valor);
	nuevo_ataque->tipo = definir_tipo(tipo);

	if (nuevo_ataque->poder == 0) {
		free(nuevo_ataque);
		return NULL;
	}

	return nuevo_ataque;
}