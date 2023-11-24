#include "jugador.h"

jugador_t *jugador_crear(){
    jugador_t *nuevo_jugador = calloc(1, sizeof(jugador_t));

    nuevo_jugador->movimientos_posibles = abb_crear(comparador_abb);

    if(!nuevo_jugador || !nuevo_jugador->movimientos_posibles){
        free(nuevo_jugador);
        free(nuevo_jugador->movimientos_posibles);
        return NULL;
    }
        

    return nuevo_jugador;
}

bool jugador_cargar_pokes(jugador_t * jugador, char *nombre, lista_t * lista){

    jugador->pokemones[jugador->cant_pokes] = calloc(1, sizeof(char)*strlen(nombre)+1);

    if(!jugador->pokemones[jugador->cant_pokes])
        return false;

    strcpy(jugador->pokemones[jugador->cant_pokes], nombre);
    jugador->cant_pokes++;

    struct almacenador almacenador;
	almacenador.cantidad = 0;
	almacenador.elemento = malloc(3*sizeof(char*));

	jugada_t j;
	strcpy(j.pokemon, nombre);

	if(!almacenador.elemento)
		return false;

	pokemon_t *poke = lista_buscar_elemento(lista, comparador_poke,(void*) nombre);
	
	con_cada_ataque(poke, buscar_ataques, (void *)&almacenador);

	for(int i = 0; i < 3; i++){
		strcpy(j.ataque, almacenador.elemento[i]);
		char * clave = crear_clave(j);
		abb_insertar(jugador->movimientos_posibles, (void*)clave);
	}

	free(almacenador.elemento[0]);
	free(almacenador.elemento[1]);
	free(almacenador.elemento[2]);
	free(almacenador.elemento);
	
	return true;
}