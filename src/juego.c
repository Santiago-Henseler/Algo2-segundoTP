#include "juego.h"
#include "juego_aux.h"

struct juego{
	informacion_pokemon_t *ip;
	lista_t *lista_poke;
	int rondas;
	struct jugador *j1;
	struct jugador *j2;
};

struct jugador
{
	char * pokemones[3];
	int puntaje;
	hash_t* usados;
};

JUEGO_ESTADO agregar_pokemon_a_jugador(struct jugador * primero, struct jugador * segundo, const char *nombre1, const char *nombre2, const char *nombre3){

	primero->pokemones[0] = calloc(1, sizeof(char)*strlen(nombre1)+1);
	primero->pokemones[1] = calloc(1, sizeof(char)*strlen(nombre2)+1);
	segundo->pokemones[2] = calloc(1, sizeof(char)*strlen(nombre3)+1);

	if(!primero->pokemones[0] || !primero->pokemones[1] || !segundo->pokemones[2])
		return ERROR_GENERAL;	

	strcpy(primero->pokemones[0], nombre1);
	strcpy(primero->pokemones[1], nombre2);
	strcpy(segundo->pokemones[2], nombre3);

	return TODO_OK;
}

juego_t *juego_crear()
{
	struct juego * nuevo_juego = calloc(1, sizeof(struct juego)); 
	nuevo_juego->j1 = calloc(1, sizeof(struct jugador));
	nuevo_juego->j2 = calloc(1, sizeof(struct jugador));
	nuevo_juego->j1->usados = hash_crear(9);
	nuevo_juego->j2->usados = hash_crear(9);
	
	if(!nuevo_juego || !nuevo_juego->j1 || !nuevo_juego->j2 || !nuevo_juego->j1->usados || !nuevo_juego->j2->usados)
		return NULL;

	return nuevo_juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if(!juego)
		return ERROR_GENERAL;

	informacion_pokemon_t *ip = pokemon_cargar_archivo(archivo);

	if(!ip)
		return ERROR_GENERAL;

	if(pokemon_cantidad(ip) < 4)
		return POKEMON_INSUFICIENTES;

	juego->ip = ip;

	juego->lista_poke = juego_listar_pokemon(juego);

	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if(!juego)
		return NULL;

	lista_t *lista = lista_crear();

	if(!lista)
		return NULL;

	if(con_cada_pokemon(juego->ip, agregar_pokemon_a_lista, lista) < pokemon_cantidad(juego->ip))
		return NULL;

	return lista;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador, const char *nombre1, const char *nombre2, const char *nombre3)
{
	if(!juego || !nombre1 || !nombre2 || !nombre3)
		return ERROR_GENERAL;

	if(strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3) == 0)
		return POKEMON_REPETIDO;

	if(!pokemon_buscar(juego->ip, nombre1) || !pokemon_buscar(juego->ip, nombre2) || !pokemon_buscar(juego->ip, nombre3))
		return POKEMON_INEXISTENTE;

	if(jugador == JUGADOR1)
		return agregar_pokemon_a_jugador(juego->j1, juego->j2, nombre1, nombre2, nombre3);
	
	return agregar_pokemon_a_jugador(juego->j2, juego->j1, nombre1, nombre2, nombre3);
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1, jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;

	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	
	if(!juego)
		return resultado;

	pokemon_t *poke1 = pokemon_buscar(juego->ip, jugada_jugador1.pokemon);
	pokemon_t *poke2 = pokemon_buscar(juego->ip, jugada_jugador2.pokemon);

	if(!poke1 || !poke2)
		return resultado;

	const struct ataque * poke1_ataque = pokemon_buscar_ataque(poke1, jugada_jugador1.ataque);
	const struct ataque * poke2_ataque = pokemon_buscar_ataque(poke2, jugada_jugador2.ataque);

	if(!poke1_ataque || !poke2_ataque)
		return resultado;

	char * clave1 = crear_clave(jugada_jugador1);
	char * clave2 = crear_clave(jugada_jugador2);

	if(hash_contiene(juego->j1->usados, clave1) || hash_contiene(juego->j2->usados, clave2)){
		free(clave1);
		free(clave2);
		return resultado;
	}
		
	resultado.jugador1 = clasificar_ataque(poke1_ataque->tipo, poke2_ataque->tipo);
	resultado.jugador2 = clasificar_ataque(poke2_ataque->tipo, poke1_ataque->tipo);

	juego->j1->puntaje += puntear_ataque(resultado.jugador1, poke1_ataque);
	juego->j2->puntaje += puntear_ataque(resultado.jugador2, poke2_ataque);

	juego->rondas++;

	hash_insertar(juego->j1->usados, clave1, (void*)clave1, NULL);
	hash_insertar(juego->j2->usados, clave2, (void*)clave2, NULL);

	free(clave1);
	free(clave2);

	return resultado;
}

int juego_obtener_puntaje(juego_t *juego, JUGADOR jugador)
{
	if(!juego)
		return 0;	

	if(jugador == JUGADOR1)
		return juego->j1->puntaje;
	return juego->j2->puntaje;
}

bool juego_finalizado(juego_t *juego)
{
	if(juego->rondas == 9 || !juego)
		return true;

	return false;
}

void juego_destruir(juego_t *juego)
{
	if(!juego)
		return;

	free(juego->j1->pokemones[0]);
	free(juego->j1->pokemones[1]);
	free(juego->j1->pokemones[2]);

	hash_destruir_todo(juego->j1->usados, free);
	free(juego->j1);
	
	free(juego->j2->pokemones[0]);
	free(juego->j2->pokemones[1]);
	free(juego->j2->pokemones[2]);

	hash_destruir_todo(juego->j2->usados, free);
	free(juego->j2);

	lista_destruir_todo(juego->lista_poke, free);

	pokemon_destruir_todo(juego->ip);

	free(juego);
}
