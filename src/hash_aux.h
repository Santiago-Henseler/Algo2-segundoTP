#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"

#define FACTOR_CARGA_MAXIMO 0.7

struct par {
	char *clave;
	void *valor;
	struct par *siguiente;
};

struct hash {
	struct par **tabla;
	size_t cantidad;
	size_t capacidad;
};

unsigned int funcion_hash(const char *clave);

struct par *crear_par(const char *clave, void *elemento);

hash_t *insertar_par(hash_t *hash, struct par *par, void **anterior);

hash_t *rehash(hash_t *hash);

struct par *quitar_recu(hash_t *hash, struct par *actual, const char *clave,
			void **elemento);

void destruir_recu(struct par *actual, void (*f)(void *));