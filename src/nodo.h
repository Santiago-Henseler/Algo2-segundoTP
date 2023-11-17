#ifndef __NODO_H__
#define __NODO_H_

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct nodo {
	void *elemento;
	struct nodo *siguiente;
} nodo_t;

//PRE: el nodo recibido es no nulo y la posición mayor a 0
//POST: devuelve un puntero al nodo en la posición pedida
nodo_t *posicion_nodo(nodo_t *nodo, int posicion);

//PRE:-
//POST: devuelve un puntero a un nodo inicializado con el elemento pasado por parametro
nodo_t *nuevo_nodo(void *elemento);

#endif /*__NODO_H_*/