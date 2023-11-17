#include "lista.h"
#include "pokemon.h"
#include "adversario.h"
#include "hash.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

struct adversario
{
	lista_t *lista_poke;
	hash_t* usados;
	char * pokemones[3];
};

adversario_t *adversario_crear(lista_t *pokemon)
{
	struct adversario *nuevo = calloc(1, sizeof(struct adversario));
	nuevo->usados = hash_crear(9);

	if(!nuevo || !pokemon)
		return NULL;

	nuevo->lista_poke = pokemon;

	return nuevo;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1, char **nombre2, char **nombre3)
{
	srand((unsigned)time(NULL));
	
	size_t tamanio = lista_tamanio(adversario->lista_poke);

	pokemon_t *poke1 = lista_elemento_en_posicion(adversario->lista_poke,  (size_t) rand() % tamanio);
	pokemon_t *poke2 = lista_elemento_en_posicion(adversario->lista_poke, (size_t)  rand() % tamanio);
	pokemon_t *poke3 = lista_elemento_en_posicion(adversario->lista_poke, (size_t) rand() % tamanio);

	if(!poke1 || !poke2 || !poke3)
		return false;

	const char * poke1_nombre = pokemon_nombre(poke1);
	const char * poke2_nombre = pokemon_nombre(poke2);
	const char * poke3_nombre = pokemon_nombre(poke3);

	if(strcmp(poke1_nombre, poke2_nombre) == 0 || strcmp(poke1_nombre, poke3_nombre) == 0 || strcmp(poke2_nombre, poke3_nombre) == 0){
		adversario_seleccionar_pokemon(adversario, nombre1, nombre2, nombre3);
		return true;
	}

	*nombre1 = calloc(1, sizeof(char)*strlen(poke1_nombre)+1);
	*nombre2 = calloc(1, sizeof(char)*strlen(poke2_nombre)+1);
	*nombre3 = calloc(1, sizeof(char)*strlen(poke3_nombre)+1);

	if(!nombre1 || !nombre2 || !nombre3)
		return false;

	strcpy(*nombre1, poke1_nombre);
	strcpy(*nombre2, poke2_nombre);
	strcpy(*nombre3, poke3_nombre);
	
	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1, char *nombre2, char *nombre3)
{
	adversario->pokemones[0] = calloc(1, sizeof(char)*strlen(nombre1)+1);
	adversario->pokemones[1] = calloc(1, sizeof(char)*strlen(nombre2)+1);
	adversario->pokemones[2] = calloc(1, sizeof(char)*strlen(nombre3)+1);

	if(!adversario->pokemones[0] || !adversario->pokemones[1] || !adversario->pokemones[2])
		return false;

	strcpy(adversario->pokemones[0], nombre1);
	strcpy(adversario->pokemones[1], nombre2);
	strcpy(adversario->pokemones[2], nombre3);

	return true;
}

/*
void buscar_ataques(pokemon_t * poke, char *ataques){


	pokemon_buscar_ataque()
}
*/
jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	/*
	srand((unsigned)time(NULL));

	int posicion = rand() % 3;

	char * poke = adversario->pokemones[posicion];
*/
	jugada_t j = {.pokemon = "", .ataque = ""};
	
	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	adversario_proxima_jugada(a);
}

void adversario_destruir(adversario_t *adversario)
{
}
