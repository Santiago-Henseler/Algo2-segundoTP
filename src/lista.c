#include "lista.h"
#include "nodo.h"

struct lista {
	nodo_t *nodo_inicio;
	size_t cant_nodos;
	nodo_t *nodo_fin;
};

struct lista_iterador {
	nodo_t *actual;
	struct lista *lista;
};

lista_t *lista_crear()
{
	struct lista *nueva_lista = calloc(1, sizeof(struct lista));

	if (nueva_lista == NULL) {
		return NULL;
	}

	return nueva_lista;
}

lista_t *lista_insertar(lista_t *lista, void *elemento)
{
	if (lista == NULL)
		return NULL;

	nodo_t *nuevo = nuevo_nodo(elemento);

	if (nuevo == NULL)
		return NULL;

	if (lista_vacia(lista)) {
		lista->nodo_inicio = nuevo;
		lista->nodo_fin = nuevo;
	} else {
		lista->nodo_fin->siguiente = nuevo;
		lista->nodo_fin = nuevo;
	}

	lista->cant_nodos++;
	return lista;
}

lista_t *lista_insertar_en_posicion(lista_t *lista, void *elemento,
				    size_t posicion)
{
	if (lista == NULL)
		return NULL;

	if (lista->cant_nodos <= posicion) {
		return lista_insertar(lista, elemento);
	}

	nodo_t *nuevo = nuevo_nodo(elemento);

	if (nuevo == NULL)
		return NULL;

	if (posicion == 0) {
		nuevo->siguiente = lista->nodo_inicio;
		lista->nodo_inicio = nuevo;
	} else {
		nodo_t *posicion_a_insertar =
			posicion_nodo(lista->nodo_inicio, (int)posicion);

		nuevo->siguiente = posicion_a_insertar->siguiente;
		posicion_a_insertar->siguiente = nuevo;
	}

	lista->cant_nodos++;
	return lista;
}

void *lista_quitar(lista_t *lista)
{
	if (lista_vacia(lista))
		return NULL;

	nodo_t *nodo_borrado = lista->nodo_fin;

	void *elemento_borrado = lista->nodo_fin->elemento;

	nodo_t *ante_ultimo =
		posicion_nodo(lista->nodo_inicio, ((int)lista->cant_nodos) - 1);

	if (ante_ultimo == NULL) {
		return NULL;
	}

	lista->nodo_fin = ante_ultimo;
	lista->nodo_fin->siguiente = NULL;

	lista->cant_nodos--;
	free(nodo_borrado);

	return elemento_borrado;
}

void *lista_quitar_de_posicion(lista_t *lista, size_t posicion)
{
	if (lista_vacia(lista))
		return NULL;

	if (lista->cant_nodos < posicion) {
		return lista_quitar(lista);
	}

	if (posicion == 0) {
		void *aux = lista->nodo_inicio->elemento;
		nodo_t *nodo_borrado = lista->nodo_inicio;

		lista->nodo_inicio = lista->nodo_inicio->siguiente;

		free(nodo_borrado);
		lista->cant_nodos--;

		return aux;
	}

	nodo_t *nuevo_nodo_fin =
		posicion_nodo(lista->nodo_inicio, (int)posicion);

	if (nuevo_nodo_fin == NULL) {
		return NULL;
	}

	nodo_t *nodo_borrado = nuevo_nodo_fin->siguiente;
	void *aux = nuevo_nodo_fin->siguiente->elemento;

	nuevo_nodo_fin->siguiente = nuevo_nodo_fin->siguiente->siguiente;

	free(nodo_borrado);
	lista->cant_nodos--;

	return aux;
}

void *lista_elemento_en_posicion(lista_t *lista, size_t posicion)
{
	if (lista_tamanio(lista) <= posicion || lista_vacia(lista))
		return NULL;

	if (posicion == 0)
		return lista->nodo_inicio->elemento;

	nodo_t *nodo_en_pos =
		posicion_nodo(lista->nodo_inicio, (int)posicion + 1);

	return nodo_en_pos->elemento;
}

void *lista_buscar_elemento(lista_t *lista, int (*comparador)(void *, void *),
			    void *contexto)
{
	if (comparador == NULL || lista_vacia(lista))
		return NULL;

	nodo_t *nodo = lista->nodo_inicio;

	while (comparador(nodo->elemento, contexto) != 0 &&
	       nodo->siguiente != NULL) {
		nodo = nodo->siguiente;
	}

	if (comparador(nodo->elemento, contexto) == 0)
		return nodo->elemento;

	if (nodo->siguiente == NULL)
		return NULL;

	return NULL;
}

void *lista_primero(lista_t *lista)
{
	if (lista_vacia(lista))
		return NULL;

	return lista->nodo_inicio->elemento;
}

void *lista_ultimo(lista_t *lista)
{
	if (lista_vacia(lista))
		return NULL;

	return lista->nodo_fin->elemento;
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL || lista_tamanio(lista) == 0)
		return true;

	return false;
}

size_t lista_tamanio(lista_t *lista)
{
	if (lista == NULL)
		return 0;

	return lista->cant_nodos;
}

void lista_destruir(lista_t *lista)
{
	if (lista == NULL)
		return;

	if (lista_vacia(lista)) {
		free(lista);
		return;
	}

	nodo_t *nodo = lista->nodo_inicio;
	nodo_t *aux = NULL;

	while (nodo != NULL) {
		aux = nodo->siguiente;
		free(nodo);
		nodo = aux;
	}

	free(lista);
}

void lista_destruir_todo(lista_t *lista, void (*funcion)(void *))
{
	if (lista == NULL)
		return;

	if (lista_vacia(lista)) {
		free(lista);
		return;
	}

	if (funcion == NULL) {
		lista_destruir(lista);
		return;
	}

	nodo_t *nodo = lista->nodo_inicio;
	nodo_t *aux = NULL;

	while (nodo != NULL) {
		aux = nodo->siguiente;
		funcion(nodo->elemento);
		free(nodo);
		nodo = aux;
	}

	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL)
		return NULL;

	struct lista_iterador *it = calloc(1, sizeof(struct lista_iterador));

	if (it == NULL)
		return NULL;

	if (!lista_vacia(lista)) {
		it->actual = lista->nodo_inicio;
		it->actual->elemento = lista->nodo_inicio->elemento;
	} else {
		it->actual = NULL;
	}

	it->lista = lista;

	return it;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t *iterador)
{
	if (iterador == NULL) {
		return false;
	}
	if (lista_vacia(iterador->lista)) {
		return false;
	}
	if (iterador->actual == NULL)
		return false;

	return true;
}

bool lista_iterador_avanzar(lista_iterador_t *iterador)
{
	if (!lista_iterador_tiene_siguiente(iterador))
		return false;

	iterador->actual = iterador->actual->siguiente;

	return iterador->actual != NULL;
}

void *lista_iterador_elemento_actual(lista_iterador_t *iterador)
{
	if (iterador == NULL || iterador->actual == NULL)
		return NULL;

	return iterador->actual->elemento;
}

void lista_iterador_destruir(lista_iterador_t *iterador)
{
	free(iterador);
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*funcion)(void *, void *),
			       void *contexto)
{
	if (lista_vacia(lista) || funcion == NULL)
		return 0;

	size_t i = 1;
	nodo_t *actual = lista->nodo_inicio;

	while (funcion(actual->elemento, contexto) &&
	       actual->siguiente != NULL) {
		i++;
		actual = actual->siguiente;
	}

	return i;
}