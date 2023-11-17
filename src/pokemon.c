#include "pokemon.h"
#include "ataque.h"
#include "cargar_pokemones.h"

struct pokemon {
	struct ataque **ataques;
	enum TIPO tipo;
	int cantidad_ataques;
	char nombre[MAX_NOMBRE];
};

struct info_pokemon {
	struct pokemon **pokemones;
	int cantidad_pokemones;
};

// Defino funciones auxiliares

// Pre: Recibe un puntero a struct info_pokemon no NULL
// Post: Ordena alfabeticamente los pokemones recibidos
void ordenar_pokemones(struct info_pokemon *ip)
{
	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		int min = i;

		for (int j = i; j < ip->cantidad_pokemones; j++) {
			if (strcmp(ip->pokemones[min]->nombre,
				   ip->pokemones[j]->nombre) > 0) {
				min = j;
			}
		}

		struct pokemon *aux = ip->pokemones[min];
		ip->pokemones[min] = ip->pokemones[i];
		ip->pokemones[i] = aux;
	}
}

// Pre: Recibe un string no NULL
// Post: Devuelve un pointer a un ataque rellenado con la informacion del string
struct pokemon *cargar_nombre_pokemon(char linea[MAX_LINEA])
{
	struct pokemon *nuevo_pokemon = malloc(sizeof(struct pokemon));

	if (nuevo_pokemon == NULL)
		return NULL;

	char tipo;

	int cantidad = sscanf(linea, "%[^;];%c", nuevo_pokemon->nombre, &tipo);

	if (cantidad < 2) {
		free(nuevo_pokemon);
		return NULL;
	}

	nuevo_pokemon->tipo = definir_tipo(tipo);

	return nuevo_pokemon;
}

// Pre: Recibe un puntero astruct info_pokemon no NULL
// Post: Borra los pokemones no validos
void validar_pokemones_leidos(struct info_pokemon *ip)
{
	int pokemones_invalidos = 0;
	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		if (ip->pokemones[i]->cantidad_ataques < 3) {
			for (int j = 0; j < ip->pokemones[i]->cantidad_ataques;
			     j++) {
				free(ip->pokemones[i]->ataques[j]);
			}
			free(ip->pokemones[i]->ataques);
			free(ip->pokemones[i]);
			pokemones_invalidos++;
		}
	}

	ip->cantidad_pokemones -= pokemones_invalidos;

	if (ip->cantidad_pokemones == 0) {
		free(ip->pokemones);
	}
}

// Pre: Recibe un puntero a struct info_pokemon y un puntero a archivo no NULL
// Post: Carga en el struct info_pokemon todos los pokemones validos que hay en
// el archivo
void leer_pokemones(FILE *archivo, struct info_pokemon *ip)
{
	char linea[MAX_LINEA];
	bool error = false;
	int linea_recorrida = 0;

	ip->pokemones = calloc(1, sizeof(struct pokemon *));

	while (fgets(linea, 200, archivo) != NULL && !error) {
		if (linea_recorrida == 0) {
			struct pokemon **aux =
				realloc(ip->pokemones,
					((unsigned int)ip->cantidad_pokemones +
					 1) * sizeof(struct pokemon *));

			if (aux == NULL) {
				error = true;
			} else {
				ip->pokemones = aux;

				ip->pokemones[ip->cantidad_pokemones] =
					cargar_nombre_pokemon(linea);

				if (ip->pokemones[ip->cantidad_pokemones] ==
				    NULL) {
					linea_recorrida = 3;
					error = true;
				} else {
					ip->pokemones[ip->cantidad_pokemones]
						->cantidad_ataques = 0;

					struct ataque **aux2 = calloc(
						3, sizeof(struct ataque));

					if (aux2 == NULL) {
						free(ip->pokemones
							     [ip->cantidad_pokemones]);
						error = true;
					} else {
						ip->pokemones
							[ip->cantidad_pokemones]
								->ataques =
							aux2;
					}
				}
			}
		} else {
			ip->pokemones[ip->cantidad_pokemones]
				->ataques[ip->pokemones[ip->cantidad_pokemones]
						  ->cantidad_ataques] =
				cargar_ataque_pokemon(linea);

			if (ip->pokemones[ip->cantidad_pokemones]->ataques
				    [ip->pokemones[ip->cantidad_pokemones]
					     ->cantidad_ataques] == NULL) {
				linea_recorrida = 3;
				error = true;
			} else {
				ip->pokemones[ip->cantidad_pokemones]
					->cantidad_ataques++;
			}
		}

		if (linea_recorrida == 3) {
			ip->cantidad_pokemones++;
			linea_recorrida = 0;
		} else {
			linea_recorrida++;
		}
	}
	validar_pokemones_leidos(ip);
}

// Fin de funciones auxiliares

informacion_pokemon_t *pokemon_cargar_archivo(const char *path)
{
	if (path == NULL)
		return NULL;

	FILE *archivo = fopen(path, "r");

	if (archivo == NULL)
		return NULL;

	struct info_pokemon *info_pokemones =
		malloc(sizeof(struct info_pokemon));

	if (info_pokemones == NULL) {
		fclose(archivo);
		free(info_pokemones);
		return NULL;
	}

	info_pokemones->cantidad_pokemones = 0;

	leer_pokemones(archivo, info_pokemones);

	fclose(archivo);

	if (info_pokemones->cantidad_pokemones == 0) {
		free(info_pokemones);
		return NULL;
	}
	return info_pokemones;
}

pokemon_t *pokemon_buscar(informacion_pokemon_t *ip, const char *nombre)
{
	if (ip == NULL || nombre == NULL)
		return NULL;

	struct pokemon *aux = NULL;
	bool encontrado = false;

	for (int i = 0; i < ip->cantidad_pokemones && !encontrado; i++) {
		if (strcmp(ip->pokemones[i]->nombre, nombre) == 0) {
			aux = ip->pokemones[i];
			encontrado = true;
		}
	}

	return aux;
}

int pokemon_cantidad(informacion_pokemon_t *ip)
{
	if (ip == NULL)
		return 0;

	return ip->cantidad_pokemones;
}

const char *pokemon_nombre(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NULL;

	return pokemon->nombre;
}

enum TIPO pokemon_tipo(pokemon_t *pokemon)
{
	if (pokemon == NULL)
		return NORMAL;

	return pokemon->tipo;
}

const struct ataque *pokemon_buscar_ataque(pokemon_t *pokemon,
					   const char *nombre)
{
	if (pokemon == NULL)
		return NULL;

	struct ataque *aux = NULL;
	bool encontrado = false;

	for (int i = 0; i < pokemon->cantidad_ataques && !encontrado; i++) {
		if (strcmp(pokemon->ataques[i]->nombre, nombre) == 0) {
			aux = pokemon->ataques[i];
			encontrado = true;
		}
	}

	return aux;
}

int con_cada_pokemon(informacion_pokemon_t *ip, void (*f)(pokemon_t *, void *),
		     void *aux)
{
	if (ip == NULL || f == NULL)
		return 0;

	ordenar_pokemones(ip);

	for (int i = 0; i < ip->cantidad_pokemones; i++) {
		f(ip->pokemones[i], aux);
	}

	return ip->cantidad_pokemones;
}

int con_cada_ataque(pokemon_t *pokemon,
		    void (*f)(const struct ataque *, void *), void *aux)
{
	if (pokemon == NULL || f == NULL)
		return 0;

	for (int i = 0; i < pokemon->cantidad_ataques; i++) {
		f(pokemon->ataques[i], aux);
	}

	return pokemon->cantidad_ataques;
}

void pokemon_destruir_todo(informacion_pokemon_t *ip)
{
	if (ip != NULL) {
		for (int i = 0; i < ip->cantidad_pokemones; i++) {
			for (int j = 0; j < ip->pokemones[i]->cantidad_ataques;
			     j++) {
				free(ip->pokemones[i]->ataques[j]);
			}
			free(ip->pokemones[i]->ataques);
			free(ip->pokemones[i]);
		}

		free(ip->pokemones);
		free(ip);
	}
}