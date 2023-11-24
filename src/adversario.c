#include "lista.h"
#include "pokemon.h"
#include "juego.h"
#include "juego_aux.h"
#include "ataque.h"
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
	nuevo->usados = hash_crear(15);

	if(!nuevo || !pokemon || !nuevo->usados){
		free(nuevo);
		hash_destruir(nuevo->usados);
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
	pokemon_t *poke2 = lista_elemento_en_posicion(adversario->lista_poke, (size_t)  rand() % tamanio);
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

jugada_t adversario_proxima_jugada(adversario_t *adversario)
{
	time_t t;
	jugada_t j;
	struct almacenador almacenador;
	almacenador.cantidad = 0;
	almacenador.elemento = malloc(3*sizeof(char));

	if(!almacenador.elemento){
		strcpy(j.ataque, "");
		strcpy(j.pokemon,"");
		return j;
	}
		
	srand((unsigned)time(&t));

	char * nombre = adversario->pokemones[rand() % 3];

	pokemon_t *poke = lista_buscar_elemento(adversario->lista_poke, comparador, nombre);

	con_cada_ataque(poke, buscar_ataques, (void *)&almacenador);

	strcpy(j.pokemon , nombre);
	strcpy(j.ataque, (char *)almacenador.elemento[rand() % almacenador.cantidad]);

	char * clave = crear_clave(j);

	if(hash_contiene(adversario->usados, clave)){
		free(clave);
		free(almacenador.elemento[0]);
		free(almacenador.elemento[1]);
		free(almacenador.elemento[2]);
		free(almacenador.elemento);
		return adversario_proxima_jugada(adversario);
	}

	hash_insertar(adversario->usados, clave, (void*)clave,NULL);

	free(clave);
	free(almacenador.elemento[0]);
	free(almacenador.elemento[1]);
	free(almacenador.elemento[2]);
	free(almacenador.elemento);

	return j;
}

void adversario_informar_jugada(adversario_t *a, jugada_t j)
{
	
}

void adversario_destruir(adversario_t *adversario)
{
	hash_destruir_todo(adversario->usados, free);
	lista_destruir_todo(adversario->lista_poke, free);

	free(adversario->pokemones[0]);
	free(adversario->pokemones[1]);
	free(adversario->pokemones[2]);

	free(adversario);
}
