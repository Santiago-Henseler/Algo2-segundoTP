#include "juego.h"
#include "juego_aux.h"

struct juego{
	informacion_pokemon_t *ip;
	lista_t *lista_poke;
	int rondas;
	jugador_t *j1;
	jugador_t *j2;
};

juego_t *juego_crear()
{
	struct juego * nuevo_juego = calloc(1, sizeof(struct juego)); 
	nuevo_juego->j1 = jugador_crear();
	nuevo_juego->j2 = jugador_crear();

	if(!nuevo_juego || !nuevo_juego->j1 || !nuevo_juego->j2){
		free(nuevo_juego);
		jugador_destruir(nuevo_juego->j1);
		jugador_destruir(nuevo_juego->j2);
		return NULL;
	}
		
	return nuevo_juego;
}

JUEGO_ESTADO juego_cargar_pokemon(juego_t *juego, char *archivo)
{
	if(!juego)
		return ERROR_GENERAL;

	informacion_pokemon_t *ip = pokemon_cargar_archivo(archivo);

	if(!ip)
		return ERROR_GENERAL;

	if(pokemon_cantidad(ip) < 4){
		pokemon_destruir_todo(ip);
		return POKEMON_INSUFICIENTES;
	}

	juego->ip = ip;

	lista_t *lista = lista_crear();

	if(!lista){
		pokemon_destruir_todo(ip);
		return ERROR_GENERAL;
	}
		
	if(con_cada_pokemon(juego->ip, agregar_pokemon_a_lista, lista) < pokemon_cantidad(juego->ip)){
		lista_destruir(lista);
		pokemon_destruir_todo(ip);
		return ERROR_GENERAL;
	}
		
	juego->lista_poke = lista;
	
	return TODO_OK;
}

lista_t *juego_listar_pokemon(juego_t *juego)
{
	if(!juego)
		return NULL;

	return juego->lista_poke;
}

JUEGO_ESTADO juego_seleccionar_pokemon(juego_t *juego, JUGADOR jugador, const char *nombre1, const char *nombre2, const char *nombre3)
{
	if(!juego || !nombre1 || !nombre2 || !nombre3)
		return ERROR_GENERAL;

	if(strcmp(nombre1, nombre2) == 0 || strcmp(nombre1, nombre3) == 0 || strcmp(nombre2, nombre3) == 0)
		return POKEMON_REPETIDO;

	if(!pokemon_buscar(juego->ip, nombre1) || !pokemon_buscar(juego->ip, nombre2) || !pokemon_buscar(juego->ip, nombre3))
		return POKEMON_INEXISTENTE;

	if(jugador == JUGADOR1){
		if(!jugador_cargar_pokes(juego->j1, (char *)nombre1,juego->lista_poke)|| !jugador_cargar_pokes(juego->j1, (char *)nombre2 ,juego->lista_poke) || !jugador_cargar_pokes(juego->j2, (char *)nombre3 ,juego->lista_poke))
			return ERROR_GENERAL;
		return TODO_OK;
	}	
		
	if(!jugador_cargar_pokes(juego->j2,(char *)nombre1,juego->lista_poke)|| !jugador_cargar_pokes(juego->j2, (char *)nombre2 ,juego->lista_poke) || !jugador_cargar_pokes(juego->j1, (char *)nombre3 ,juego->lista_poke))
		return ERROR_GENERAL;
	return TODO_OK;
}

resultado_jugada_t juego_jugar_turno(juego_t *juego, jugada_t jugada_jugador1, jugada_t jugada_jugador2)
{
	resultado_jugada_t resultado;

	resultado.jugador1 = ATAQUE_ERROR;
	resultado.jugador2 = ATAQUE_ERROR;
	
	if(!juego)
		return resultado;

	char * jugada1 = crear_clave(jugada_jugador1);
	char * jugada2 = crear_clave(jugada_jugador2);

	if(!abb_buscar(juego->j1->movimientos_posibles, (void*)jugada1) || !abb_buscar(juego->j2->movimientos_posibles, (void*)jugada2)){
		free(jugada1);
		free(jugada2);
		return resultado;
	}

	pokemon_t *poke1 = pokemon_buscar(juego->ip, jugada_jugador1.pokemon);
	pokemon_t *poke2 = pokemon_buscar(juego->ip, jugada_jugador2.pokemon);

	const struct ataque * poke1_ataque = pokemon_buscar_ataque(poke1, jugada_jugador1.ataque);
	const struct ataque * poke2_ataque = pokemon_buscar_ataque(poke2, jugada_jugador2.ataque);

	resultado.jugador1 = clasificar_ataque(poke1_ataque->tipo, poke2_ataque->tipo);
	resultado.jugador2 = clasificar_ataque(poke2_ataque->tipo, poke1_ataque->tipo);

	juego->j1->puntaje += puntear_ataque(resultado.jugador1, poke1_ataque);
	juego->j2->puntaje += puntear_ataque(resultado.jugador2, poke2_ataque);

	juego->rondas++;

	void * anterior1 = abb_quitar(juego->j1->movimientos_posibles,(void*)jugada1);
	void * anterior2 = abb_quitar(juego->j2->movimientos_posibles,(void*)jugada2);

	free(anterior1);
	free(anterior2);
	free(jugada1);
	free(jugada2);

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
		
	jugador_destruir(juego->j1);

	jugador_destruir(juego->j2);

	lista_destruir(juego->lista_poke);

	pokemon_destruir_todo(juego->ip);

	free(juego);
}
