#include "hash_aux.h"

unsigned int funcion_hash(const char *clave)
{
	int hash = 5381;
	int c;
	while ((c = *clave++))
		hash = ((hash << 5) + hash) + c;
	return (unsigned)hash;
}

struct par *crear_par(const char *clave, void *elemento)
{
	struct par *nuevo_par = calloc(1, sizeof(struct par));

	if (!nuevo_par)
		return NULL;

	nuevo_par->clave = calloc(1, strlen(clave) + 1);

	if (!nuevo_par->clave) {
		free(nuevo_par);
		return NULL;
	}

	strcpy(nuevo_par->clave, clave);

	nuevo_par->valor = elemento;
	return nuevo_par;
}

hash_t *insertar_par(hash_t *hash, struct par *par, void **anterior)
{
	int posicion =
		(int)(funcion_hash(par->clave) % (unsigned)hash->capacidad);

	struct par *actual = hash->tabla[posicion];

	bool insertado = false;

	while (actual && !insertado) {
		if (strcmp(par->clave, actual->clave) == 0) {
			if (anterior)
				*anterior = actual->valor;
			actual->valor = par->valor;
			free(par->clave);
			free(par);
			insertado = true;
		}
		actual = actual->siguiente;
	}

	if (!insertado) {
		if (anterior)
			*anterior = NULL;
		par->siguiente = hash->tabla[posicion];
		hash->tabla[posicion] = par;
		hash->cantidad++;
		insertado = true;
	}

	return hash;
}

hash_t *rehash(hash_t *hash)
{
	struct par **vieja_tabla = hash->tabla;

	hash->capacidad *= 2;
	hash->tabla = calloc(hash->capacidad, sizeof(struct par));
	if (!hash->tabla)
		return NULL;

	hash->cantidad = 0;

	for (int i = 0; i < hash->capacidad; i++) {
		struct par *actual = vieja_tabla[i];

		while (actual) {
			struct par *aux = actual->siguiente;
			insertar_par(hash, actual, NULL);
			actual = aux;
		}
	}

	free(vieja_tabla);
	return hash;
}

struct par *quitar_recu(hash_t *hash, struct par *actual, const char *clave,
			void **elemento)
{
	if (!actual)
		return NULL;

	if (strcmp(clave, actual->clave) == 0) {
		struct par *aux = actual->siguiente;
		*elemento = actual->valor;
		free(actual->clave);
		free(actual);
		hash->cantidad--;
		return aux;
	}

	actual->siguiente =
		quitar_recu(hash, actual->siguiente, clave, elemento);

	return actual;
}

void destruir_recu(struct par *actual, void (*f)(void *))
{
	if (actual) {
		destruir_recu(actual->siguiente, f);
		if (f)
			f(actual->valor);
		free(actual->clave);
		free(actual);
	}
	return;
}