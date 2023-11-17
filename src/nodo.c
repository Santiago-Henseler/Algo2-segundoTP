#include "nodo.h"

nodo_t *posicion_nodo(nodo_t *nodo, int posicion)
{
	if (posicion <= 1)
		return nodo;

	return posicion_nodo(nodo->siguiente, posicion - 1);
}

nodo_t *nuevo_nodo(void *elemento)
{
	nodo_t *nodo = calloc(1, sizeof(nodo_t));

	if (nodo == NULL)
		return NULL;

	nodo->elemento = elemento;

	return nodo;
}