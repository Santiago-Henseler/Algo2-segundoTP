#include "src/tipo.h"
#include <stdio.h>
#include <string.h>
#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "pa2m.h"

#define ARCHIVO_OK "ejemplos/correcto.txt"
#define ARCHIVO_CORTO "ejemplos/corto.txt"
#define ARCHIVO_INEXISTENTE "/AsddA/ASD/ASD/ASDasD/ASD/ASD7aSD/ASD/a"

void crear_juego()
{
	juego_t *j = NULL;

	pa2m_afirmar((j = juego_crear()) != NULL,
		     "juego_crear devuelve un juego no nulo");
	pa2m_afirmar(lista_tamanio(juego_listar_pokemon(j)) == 0,
		     "El listado de pokemones es vacío");
	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR1) == 0,
		     "El puntaje del jugador 1 es 0");
	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR2) == 0,
		     "El puntaje del jugador 2 es 0");
	pa2m_afirmar(juego_finalizado(j) == false,
		     "El juego no está finalizado");
	juego_destruir(j);
}

void cargar_archivo()
{
	juego_t *j = juego_crear();

	pa2m_afirmar(juego_cargar_pokemon(j, ARCHIVO_INEXISTENTE) ==
			     ERROR_GENERAL,
		     "Cargar un archivo inexistente resulta en ERROR_GENERAL");
	pa2m_afirmar(lista_tamanio(juego_listar_pokemon(j)) == 0,
		     "El listado de pokemones es vacío");
	pa2m_afirmar(
		juego_cargar_pokemon(j, ARCHIVO_CORTO) == POKEMON_INSUFICIENTES,
		"Cargar un archivo con menos pokemon que los necesarios resulta en ERROR_GENERAL");
	pa2m_afirmar(lista_tamanio(juego_listar_pokemon(j)) == 0,
		     "El listado de pokemones es vacío");
	pa2m_afirmar(juego_cargar_pokemon(j, ARCHIVO_OK) == TODO_OK,
		     "Cargar un archivo existente resulta en TODO_OK");
	pa2m_afirmar(lista_tamanio(juego_listar_pokemon(j)) == 6,
		     "El listado de pokemones tiene el tamaño correcto");

	juego_destruir(j);
}

void seleccionar()
{
	juego_t *j = juego_crear();
	pa2m_afirmar(juego_cargar_pokemon(j, ARCHIVO_OK) == TODO_OK,
		     "Cargar un archivo existente resulta en TODO_OK");
	pa2m_afirmar(lista_tamanio(juego_listar_pokemon(j)) == 6,
		     "El listado de pokemones tiene el tamaño correcto");
	pa2m_afirmar(juego_seleccionar_pokemon(j, JUGADOR1, "Pikachu",
					       "Charmander",
					       "Pikachu") == POKEMON_REPETIDO,
		     "Un jugador no puede seleccionar pokemon repetidos");
	pa2m_afirmar(juego_seleccionar_pokemon(j, JUGADOR1, "Pikachu", "sarasa",
					       "Charmander") ==
			     POKEMON_INEXISTENTE,
		     "Un jugador no puede seleccionar pokemon inexistente");
	pa2m_afirmar(juego_seleccionar_pokemon(j, JUGADOR1, "Cacnea", "Pikachu",
					       "Charmander") == TODO_OK,
		     "Un jugador puede seleccionar 3 pokemon diferentes");
	pa2m_afirmar(
		juego_seleccionar_pokemon(j, JUGADOR2, "Charmander", "Pikachu",
					  "Cacnea") == TODO_OK,
		"Es valido que el ambos jugadores repitan pokemones entre si");

	juego_destruir(j);
}

void jugar()
{
	juego_t *j = juego_crear();

	pa2m_afirmar(juego_cargar_pokemon(j, ARCHIVO_OK) == TODO_OK,
		     "Cargar un archivo existente resulta en TODO_OK");
	pa2m_afirmar(lista_tamanio(juego_listar_pokemon(j)) == 6,
		     "El listado de pokemones tiene el tamaño correcto");
	pa2m_afirmar(
		juego_seleccionar_pokemon(j, JUGADOR1, "Pikachu", "Charmander",
					  "Cacnea") == TODO_OK,
		"Los pokemon del jugador 1 se pueden seleccionar sin problema");
	pa2m_afirmar(
		juego_seleccionar_pokemon(j, JUGADOR2, "Floatzel", "Togepi",
					  "Larvitar") == TODO_OK,
		"Los pokemon del jugador 2 se pueden seleccionar sin problema");

	jugada_t invalida1 = { .pokemon = "Pochita", .ataque = "nada" };
	jugada_t invalida2 = { .pokemon = "Pikachu", .ataque = "nada" };

	jugada_t validaj11 = { .pokemon = "Pikachu", .ataque = "Latigo" };
	jugada_t validaj21 = { .pokemon = "Floatzel", .ataque = "Retribucion" };

	jugada_t validaj12 = { .pokemon = "Pikachu", .ataque = "Chispa" };
	jugada_t validaj22 = { .pokemon = "Floatzel", .ataque = "Buceo" };

	jugada_t validaj13 = { .pokemon = "Charmander",
			       .ataque = "Lanzallamas" };
	jugada_t validaj23 = { .pokemon = "Cacnea", .ataque = "Hojas" };

	resultado_jugada_t obtenido;

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, invalida1, invalida1))
				.jugador1 == ATAQUE_ERROR,
		"Realizar una jugada inválida con ambos jugadores devuelve ATAQUE_ERROR");
	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, invalida1, validaj21))
				.jugador1 == ATAQUE_ERROR,
		"Realizar una jugada inválida con el primer jugador devuelve ATAQUE_ERROR para el jugador");
	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj11, invalida2))
				.jugador2 == ATAQUE_ERROR,
		"Realizar una jugada inválida con el segundo jugador devuelve ATAQUE_ERROR para el segundo jugador");
	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj11, validaj21))
					.jugador1 == ATAQUE_REGULAR &&
			obtenido.jugador2 == ATAQUE_REGULAR,
		"Realizar una jugada valida de tipo normal devuelve ATAQUE_REGULAR");
	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj11, validaj21))
				.jugador1 == ATAQUE_ERROR,
		"No se puede volver a utilizar el mismo ataque/pokemon dos veces");

	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR1) == 1,
		     "El puntaje del jugador 1 es 1");
	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR2) == 2,
		     "El puntaje del jugador 2 es 2");

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj12, validaj22))
				.jugador1 == ATAQUE_EFECTIVO,
		"Realizar una jugada valida de tipo ELECTRICO contra un pokemon AGUA resulta en ataque efectivo");
	pa2m_afirmar(
		obtenido.jugador2 == ATAQUE_INEFECTIVO,
		"Realizar una jugada valida de tipo AGUA contra un pokemon ELECTRICO resulta en ataque inefectivo");

	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR1) == 4,
		     "El puntaje del jugador 1 es 4 (1 + 1*3)");
	pa2m_afirmar(
		juego_obtener_puntaje(j, JUGADOR2) == 4,
		"El puntaje del jugador 2 es 4 (2 + 3/2 redondeando hacia arriba)");

	pa2m_afirmar(juego_finalizado(j) == false,
		     "El juego no está finalizado");

	pa2m_afirmar(
		(obtenido = juego_jugar_turno(j, validaj13, validaj23))
				.jugador1 == ATAQUE_EFECTIVO,
		"Realizar una jugada valida de tipo FUEGO contra un pokemon PLANTA resulta en ataque efectivo");
	pa2m_afirmar(
		obtenido.jugador2 == ATAQUE_INEFECTIVO,
		"Realizar una jugada valida de tipo PLANTA contra un pokemon FUEGO resulta en ataque inefectivo");

	pa2m_afirmar(juego_obtener_puntaje(j, JUGADOR1) == 16,
		     "El puntaje del jugador 1 es 16 (4 + 4*3)");
	pa2m_afirmar(
		juego_obtener_puntaje(j, JUGADOR2) == 5,
		"El puntaje del jugador 2 es 5 (4 + 2/2 redondeando hacia arriba)");

	pa2m_afirmar(juego_finalizado(j) == false,
		     "El juego no está finalizado");

	juego_destruir(j);
}

int main()
{
	pa2m_nuevo_grupo("------------ PRUEBAS DEL TP1 ------------");

	pa2m_nuevo_grupo("CREAR UN JUEGO");
	crear_juego();

	pa2m_nuevo_grupo("CARGAR POKEMON");
	cargar_archivo();

	pa2m_nuevo_grupo("SELECCION DE POKEMON");
	seleccionar();

	pa2m_nuevo_grupo("JUGAR");
	jugar();

	return pa2m_mostrar_reporte();
}
