#include "lista.h"
#include "pokemon.h"
#include "juego.h"
#include "juego_aux.h"
#include "ataque.h"
#include "adversario.h"
#include "jugador.h"
#include "abb.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

struct adversario
{
	lista_t *lista_poke;
	jugador_t *jugador;
};

adversario_t *adversario_crear(lista_t *pokemon)
{
	struct adversario *nuevo = calloc(1, sizeof(struct adversario));

	nuevo->jugador = jugador_crear();

	if(!nuevo || !pokemon || !nuevo->jugador){
		free(nuevo);
		return NULL;
	}
		
	nuevo->lista_poke = pokemon;

	return nuevo;
}

bool adversario_seleccionar_pokemon(adversario_t *adversario, char **nombre1, char **nombre2, char **nombre3)
{
	time_t t;
	srand((unsigned)time(&t));
	
	size_t tamanio = lista_tamanio(adversario->lista_poke);

	pokemon_t *poke1 = lista_elemento_en_posicion(adversario->lista_poke, (size_t) rand() % tamanio);
	pokemon_t *poke2 = lista_elemento_en_posicion(adversario->lista_poke, (size_t) rand() % tamanio);
	pokemon_t *poke3 = lista_elemento_en_posicion(adversario->lista_poke, (size_t) rand() % tamanio);

	if(!poke1 || !poke2 || !poke3)
		return false;

	const char * poke1_nombre = pokemon_nombre(poke1);
	const char * poke2_nombre = pokemon_nombre(poke2);
	const char * poke3_nombre = pokemon_nombre(poke3);

	*nombre1 = calloc(1, sizeof(char)*strlen(poke1_nombre)+1);
	*nombre2 = calloc(1, sizeof(char)*strlen(poke2_nombre)+1);
	*nombre3 = calloc(1, sizeof(char)*strlen(poke3_nombre)+1);

	if(!nombre1 || !nombre2 || !nombre3)
		return false;

	strcpy(*nombre1, poke1_nombre);
	strcpy(*nombre2, poke2_nombre);
	strcpy(*nombre3, poke3_nombre);

	if(strcmp(*nombre1, *nombre2) == 0){
		return adversario_seleccionar_pokemon(adversario, nombre1, nombre2, nombre3);
	}
	
	return true;
}

bool adversario_pokemon_seleccionado(adversario_t *adversario, char *nombre1, char *nombre2, char *nombre3)
{
	if(!jugador_cargar_pokes(adversario->jugador, nombre1,adversario->lista_poke)|| !jugador_cargar_pokes(adversario->jugador, nombre2 ,adversario->lista_poke) || !jugador_cargar_pokes(adversario->jugador, nombre3 ,adversario->lista_poke))
		return false;
	return true;
}

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	time_t t;
	jugada_t j;	
	srand((unsigned)time(&t+2));

	int cantidad = (int)abb_tamanio(adversario->jugador->movimientos_posibles);

	char *jugadas_validas[cantidad];

	abb_recorrer(adversario->jugador->movimientos_posibles, INORDEN, (void **)jugadas_validas, (size_t)cantidad);

	char * jugada = jugadas_validas[rand() % cantidad];

	char * nombre = strtok(jugada, ",");
	char * ataque = strtok(NULL, ",");

	strcpy(j.pokemon, nombre);
	strcpy(j.ataque, ataque);

	void * jugada_anterior = abb_quitar(adversario->jugador->movimientos_posibles, (void*)jugada);
	free(jugada_anterior);

	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	
}

void adversario_destruir(adversario_t *adversario)
{
	jugador_destruir(adversario->jugador);
	free(adversario);
}
