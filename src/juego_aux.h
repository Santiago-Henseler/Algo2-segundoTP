#ifndef JUEGO_AUX_H_
#define JUEGO_AUX_H_

#include "juego.h"
#include "tipo.h"
#include "ataque.h"
#include "pokemon.h"
#include "hash.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct almacenador{
	void **elemento;
	int cantidad;
};

int comparador(void *_actual, void * _contexto);

void buscar_ataques(const struct ataque * _ataque, void *_almacenador);

void agregar_pokemon_a_lista(pokemon_t * _pokemon, void * _lista);

char * crear_clave(jugada_t jugada);

RESULTADO_ATAQUE clasificar_ataque(enum TIPO ataque1, enum TIPO ataque2);

int puntear_ataque(RESULTADO_ATAQUE resultado, const struct ataque *ataque);

#endif // JUEGO_AUX_H_