#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <stdio.h>

/**
* Este main debe ser modificado para que el usuario pueda jugar el juego. Las
* instrucciones existentes son solamente a modo ilustrativo del funcionamiento
* muy alto nivel del juego.
*
* Las interacciones deben realizarse por entrada/salida estandar y estar validadas.
*
* Se aconseja en todo momento mostrar información relevante para el jugador (por
* ejemplo, mostrar puntaje actual y movimientos disponibles) para hacer que el
* juego sea facil de utilizar.
*/

int main(int argc, char *argv[])
{
	juego_t *juego = juego_crear();

	//Pide al usuario un nombre de archivo de pokemones
	char *archivo = pedir_archivo();

	//Carga los pokemon
	juego_cargar_pokemon(juego, archivo);

	//Crea un adversario que será utilizado como jugador 2
	adversario_t *adversario =
		adversario_crear(juego_listar_pokemon(juego));

	//Mostrar el listado de pokemones por consola para que el usuario sepa las opciones que tiene
	mostrar_pokemon_disponibles(juego);

	//Pedirle al jugador por consola que ingrese los 3 nombres de pokemon que quiere utilizar
	char *eleccionJugador1, *eleccionJugador2, *eleccionJugador3;
	jugador_seleccionar_pokemon(juego, &eleccion_adversario1,
				    &eleccion_adversario2,
				    &eleccion_adversario3);

	//pedirle al adversario que indique los 3 pokemon que quiere usar
	char *eleccionAdversario1, *eleccionAdversario2, *eleccionAdversario3;
	adversario_seleccionar_pokemon(adversario, &eleccionAdversario1,
				       &eleccionAdversario2,
				       &eleccionAdversario3);

	//Seleccionar los pokemon de los jugadores
	juego_seleccionar_pokemon(juego, JUGADOR1, eleccionJugador1,
				  eleccionJugador2, eleccionJugador3);
	juego_seleccionar_pokemon(juego, JUGADOR2, eleccionAdversario1,
				  eleccionAdversario2, eleccionAdversario3);

	//informarle al adversario cuales son los pokemon del jugador
	adversario_pokemon_seleccionado(adversario, eleccionJugador1,
					eleccionJugador2, eleccionJugador3);

	while (!juego_finalizado(juego)) {
		resultado_jugada_t resultado_ronda;

		//Pide al jugador que ingrese por consola el pokemon y ataque para la siguiente ronda
		jugada_t jugada_jugador = jugador_pedir_nombre_y_ataque();

		//Pide al adversario que informe el pokemon y ataque para la siguiente ronda
		jugada_t jugada_adversario =
			adversario_proxima_jugada(adversario);

		//jugar la ronda y después comprobar que esté todo ok, si no, volver a pedir la jugada del jugador
		resultado_ronda = juego_jugar_turno(juego, jugada_jugador,
						    jugada_adversario);

		//Si se pudo jugar el turno, le informo al adversario la jugada realizada por el jugador
		adversario_informar_jugada(adversario, jugada_jugador);
	}

	juego_destruir(juego);
}
