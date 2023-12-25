#include "abb.h"

#include "abb_aux.h"

abb_t *abb_crear(abb_comparador comparador)
{
	if (comparador == NULL)
		return NULL;

	struct abb *arbol = calloc(1, sizeof(struct abb));

	if (arbol == NULL)
		return NULL;

	arbol->comparador = comparador;

	return arbol;
}

abb_t *abb_insertar(abb_t *arbol, void *elemento)
{
	if (arbol == NULL)
		return NULL;

	if (abb_tamanio(arbol) == 0) {
		struct nodo_abb *raiz = calloc(1, sizeof(struct nodo_abb));

		if (raiz == NULL)
			return NULL;

		arbol->nodo_raiz = raiz;
		arbol->nodo_raiz->elemento = elemento;
		arbol->tamanio++;
		return arbol;
	}

	bool insertado = false;

	insertar_recursivo(arbol, arbol->nodo_raiz, elemento, &insertado);

	if (insertado)
		return arbol;

	return NULL;
}

void *abb_quitar(abb_t *arbol, void *elemento)
{
	if (abb_vacio(arbol))
		return NULL;

	almacenador_t almacenador;
	almacenador.elementos = NULL;
	almacenador.total = 0;

	arbol->nodo_raiz = quitar_recursivo(arbol, elemento, arbol->nodo_raiz,
					    &almacenador);

	if (almacenador.total == 1) {
		arbol->tamanio--;
		return almacenador.elementos;
	}

	return NULL;
}

void *abb_buscar(abb_t *arbol, void *elemento)
{
	if (abb_vacio(arbol))
		return NULL;

	struct nodo_abb *encontrado = buscar(arbol, arbol->nodo_raiz, elemento);

	if (encontrado == NULL)
		return NULL;

	return encontrado->elemento;
}

bool abb_vacio(abb_t *arbol)
{
	if (arbol == NULL)
		return true;

	if (arbol->tamanio == 0)
		return true;

	return false;
}

size_t abb_tamanio(abb_t *arbol)
{
	if (arbol == NULL)
		return 0;

	return arbol->tamanio;
}

void abb_destruir(abb_t *arbol)
{
	if (arbol == NULL) {
		return;
	}

	destruir_todo_iterativa(arbol->nodo_raiz, NULL);
	free(arbol);
}

void abb_destruir_todo(abb_t *arbol, void (*destructor)(void *))
{
	if (destructor == NULL)
		free(arbol);

	if (arbol == NULL || destructor == NULL)
		return;

	destruir_todo_iterativa(arbol->nodo_raiz, destructor);
	free(arbol);
}

size_t abb_con_cada_elemento(abb_t *arbol, abb_recorrido recorrido,
			     bool (*funcion)(void *, void *), void *aux)
{
	if (abb_vacio(arbol) || funcion == NULL) {
		return 0;
	}

	size_t recorridos = 0;

	if (recorrido == PREORDEN)
		recorrido_iterativo_preorden(arbol->nodo_raiz, funcion, aux,
					     &recorridos, arbol->tamanio);

	if (recorrido == INORDEN)
		recorrido_iterativo_inorden(arbol->nodo_raiz, funcion, aux,
					    &recorridos, arbol->tamanio);

	if (recorrido == POSTORDEN)
		recorrido_iterativo_postorden(arbol->nodo_raiz, funcion, aux,
					      &recorridos, arbol->tamanio);

	return recorridos;
}

size_t abb_recorrer(abb_t *arbol, abb_recorrido recorrido, void **array,
		    size_t tamanio_array)
{
	if (arbol == NULL || array == NULL)
		return 0;

	almacenador_t almacenador;
	almacenador.cantidad = 0;
	almacenador.total = tamanio_array;
	almacenador.elementos = array;

	abb_con_cada_elemento(arbol, recorrido, aniadir_al_array, &almacenador);

	return almacenador.cantidad;
}
