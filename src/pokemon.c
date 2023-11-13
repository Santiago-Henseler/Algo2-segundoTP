#include <stdlib.h>
#include "pokemon.h"
#include "tipo.h"
#include <stdio.h>
#include "ataque.h"
#include <string.h>

struct pokemon {
	int pone_lo_que_haga_falta;
};

struct info_pokemon {
	int aca_tambien;
};

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	return NULL;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	return NULL;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	return 0;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	return NULL;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	return FUEGO;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	return NULL;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	return 0;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	return 0;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
}
