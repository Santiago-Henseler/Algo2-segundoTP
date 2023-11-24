#include "juego_aux.h"

int comparador_poke(void *_actual, void * _contexto){
	
	pokemon_t * actual = _actual;
	const char *actual_nombre = pokemon_nombre(actual);
	char *contexto = _contexto;

	return strcmp(actual_nombre, contexto);
}

int comparador_abb(void *_a, void * _b){
	char * a = _a;
	char * b = _b;

	return strcmp(a,b);
}

void buscar_ataques(const struct ataque * _ataque, void *_almacenador){

 	struct almacenador *almacenador = _almacenador;
	char * nombre = (char *)_ataque->nombre;
	
	almacenador->elemento[almacenador->cantidad] = calloc(1, sizeof(char)*strlen(nombre)+1);

	strcpy(almacenador->elemento[almacenador->cantidad], nombre);
	almacenador->cantidad++;
}

almacenador_t * almacenar_ataques(char * nombre, lista_t *lista){
	almacenador_t *almacenador = calloc(1, sizeof(almacenador_t));
	almacenador->elemento = calloc(3,sizeof(char));

	if(!almacenador || !almacenador->elemento)
		return NULL;

	pokemon_t *poke = lista_buscar_elemento(lista, comparador_poke, nombre);

	con_cada_ataque(poke, buscar_ataques, (void *)almacenador);

	return almacenador;
}

void agregar_pokemon_a_lista(pokemon_t * _pokemon, void * _lista){
	lista_t *lista = _lista;

	 lista_insertar(lista, _pokemon);
	return;
}

char * crear_clave(jugada_t jugada){

	char * clave = calloc(1, sizeof(char)*(strlen(jugada.pokemon) + strlen(jugada.ataque) +1 ) );

	strcpy(clave, jugada.pokemon);
	strcat(clave, ",");
	strcat(clave, jugada.ataque);

	return clave;
}

RESULTADO_ATAQUE clasificar_ataque(enum TIPO ataque1, enum TIPO ataque2){	
	if(ataque1 == FUEGO && ataque2 == PLANTA)
		return ATAQUE_EFECTIVO;
	if(ataque1 == FUEGO && ataque2 == AGUA)
		return ATAQUE_INEFECTIVO;
	if(ataque1 == PLANTA && ataque2 == ROCA)
		return ATAQUE_EFECTIVO;
	if(ataque1 == PLANTA && ataque2 == FUEGO)
		return ATAQUE_INEFECTIVO;
	if(ataque1 == ROCA && ataque2 == ELECTRICO)
		return ATAQUE_EFECTIVO;
	if(ataque1 == ROCA && ataque2 == PLANTA)
		return ATAQUE_INEFECTIVO;
	if(ataque1 == ELECTRICO && ataque2 == AGUA)
		return ATAQUE_EFECTIVO;
	if(ataque1 == ELECTRICO && ataque2 == ROCA)
		return ATAQUE_INEFECTIVO;
	if(ataque1 == AGUA && ataque2 == FUEGO)
		return ATAQUE_EFECTIVO;
	if(ataque1 == AGUA && ataque2 == ELECTRICO)
		return ATAQUE_INEFECTIVO;

	return ATAQUE_REGULAR;
}

int puntear_ataque(RESULTADO_ATAQUE resultado, const struct ataque *ataque){
	
	if(resultado == ATAQUE_REGULAR)
		return (int)ataque->poder;
	
	if(resultado == ATAQUE_EFECTIVO)
		return (int)ataque->poder * 3;

	if(resultado == ATAQUE_INEFECTIVO)
		return (int)ceil((float)ataque->poder / 2);
		
	return 0;
}
