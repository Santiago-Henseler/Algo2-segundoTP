#ifndef JUEGADOR_H_
#define JUEGADOR_H_

#include "juego.h"
#include "juego_aux.h"
#include "abb.h"

typedef struct jugador
{
	char * pokemones[3];
    int cant_pokes;
	int puntaje;
	abb_t* movimientos_posibles;
}jugador_t;


jugador_t *jugador_crear();

bool jugador_cargar_pokes(jugador_t * jugador, char *nombre, lista_t * lista);

#endif