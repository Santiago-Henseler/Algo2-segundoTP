#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MOSTRAR_MENU 'm'
#define MOSTRAR_POKE 's'
#define ELEJIDOS 'p'
#define ELEJIR_POKE 'e'
#define ATACAR 'a'
#define PUNTAJE 'v'

struct data_jugador
{
	char * Pokemones[3];
	bool elejidos;
};

bool mostrar_pokes(void * _poke, void * aux){

	pokemon_t *poke = _poke; 

	printf("║               %s                     ║\n", pokemon_nombre(poke));

	return true;
}

void mostrar_todos_pokes(lista_t * lista){
	printf("Los pokemones disponibles son: \n");
	printf("\n");
	printf("╔═══════════════════◓═════════════════════╗\n");
	printf("║               POKEMONES                 ║\n");
	printf("║                                         ║\n");
	lista_con_cada_elemento(lista, mostrar_pokes, NULL);
	printf("║                                         ║\n");
	printf("╚═══════════════════◓═════════════════════╝\n\n");
}

bool cargar_archivo(juego_t *juego, char *PATH){
	JUEGO_ESTADO carga = juego_cargar_pokemon(juego, PATH);

	if(carga == ERROR_GENERAL){
		printf("Error al cargar el archivo, intentalo nuevamente\n");
		return false;
	}
	if(carga == POKEMON_INSUFICIENTES){
		printf("Error al iniciar el juego, archivo con pocos pokemones (minimo 4)\n");
		return false;
	}

	return true;
}

void mostrar_comandos(){

	system("clear");

	printf("╔═══════════════════◓═════════════════════╗\n"
		   "║                COMANDOS                 ║\n"
	       "║                                         ║\n"
		   "║           Mostrar comandos       (m)    ║\n"
		   "║         Pokemones disponibles    (s)    ║\n"
		   "║          Pokemones elejidos      (p)    ║\n"
		   "║           Elegir pokemones       (e)    ║\n"
		   "║            Realizar ataque       (a)    ║\n"
		   "║            Mostrar puntaje       (v)    ║\n"
		   "║                                         ║\n"
		   "╚═══════════════════◓═════════════════════╝\n\n");

}

void mostrar_puntaje(juego_t * juego){

	system("clear");

	printf("╔═══════════════════◓═════════════════════╗\n"
		   "║               PUNTAJES                  ║\n"
	       "║                                         ║\n"
		   "║           Tus puntos son : %i            ║\n"
		   "║                                         ║\n"
		   "║        Puntos del adversario: %i         ║\n"
		   "║                                         ║\n"
		   "╚═══════════════════◓═════════════════════╝\n\n", juego_obtener_puntaje(juego, JUGADOR1), juego_obtener_puntaje(juego, JUGADOR2));

}

void mostrar_seleccionados(struct data_jugador * jugador1){
	system("clear");

	if(!jugador1->elejidos){
		printf("Todavia no seleccionaste ningun pokemon!\n");
		return;
	}

	printf("╔═══════════════════◓═════════════════════╗\n"
		   "║             TUS POKEMONES               ║\n"
	       "║                                         ║\n"
		   "║                %s                       ║\n"
		   "║                                         ║\n"
		   "║                %s                       ║\n"
		   "║                                         ║\n"
		   "║                %s                       ║\n"
		   "║                                         ║\n"
		   "╚═══════════════════◓═════════════════════╝\n\n", jugador1->Pokemones[0], jugador1->Pokemones[1], jugador1->Pokemones[2]);

}

void elejir_poke(juego_t * juego, adversario_t *adversario, lista_t *lista, struct data_jugador *jugador1){

	system("clear");
	printf("Para iniciar el juego selecciona 3 de los pokemones disponibles\n\n");
	mostrar_todos_pokes(lista);
	printf("\n");
	printf("Indicando los nombres de esta manera: 'nombre1,nombre2,nombre3'\n");
	printf("\n");

	char linea[200];
	bool elejido = false;

	char *elejido1, *elejido2, *elejido3;

	while (!elejido)
	{
		printf("Selecciono: ");
		scanf("%s", linea);
		elejido1 = strtok(linea, ",");
		elejido2 = strtok(NULL, ",");
		elejido3 = strtok(NULL, ",");

		JUEGO_ESTADO estado = juego_seleccionar_pokemon(juego, JUGADOR1, elejido1, elejido2, elejido3);

		if(estado == TODO_OK){
			elejido = true;
		}
		else if(estado == POKEMON_REPETIDO){
			printf("Los pokemones seleccionados deben ser distintos\n");
		}
		else if(estado == POKEMON_INEXISTENTE){
			printf("Los pokemones seleccionados deben existir\n");
		}
		else if(estado == ERROR_GENERAL){
			printf("Error, debes elegir 3 pokemones respetando el formato\n");
		}
	}

	char * nombre1, *nombre2, *nombre3;

	adversario_seleccionar_pokemon(adversario, &nombre1, &nombre2, &nombre3);

	jugador1->Pokemones[0] = calloc(1, sizeof(char)*strlen(elejido1)+1);
	jugador1->Pokemones[1] = calloc(1, sizeof(char)*strlen(elejido2)+1);
	jugador1->Pokemones[2] = calloc(1, sizeof(char)*strlen(nombre3)+1);

	strcpy(jugador1->Pokemones[0], elejido1);
	strcpy(jugador1->Pokemones[1], elejido2);
	strcpy(jugador1->Pokemones[2], nombre3);
	jugador1->elejidos = true;

	if(!adversario_pokemon_seleccionado(adversario, nombre1, nombre2, elejido3)|| !jugador1->Pokemones[2] || !jugador1->Pokemones[1] || !jugador1->Pokemones[0]){
		system("clear");
		printf("Error al cargar pokemones, intentelo nuevamente!\n");
		return;
	}

	mostrar_seleccionados(jugador1);
}

void ataque(juego_t * juego, adversario_t *adversario){
	
}

void comander(char comando, lista_t* lista, juego_t * juego, adversario_t *adversario, struct data_jugador *jugador1){

	comando = (char)tolower(comando);

	if(comando == MOSTRAR_MENU)
		mostrar_comandos();
	else if(comando == MOSTRAR_POKE){
		system("clear");
		mostrar_todos_pokes(lista);
	}
	else if(comando == ELEJIDOS)
		mostrar_seleccionados(jugador1);
	else if(comando == ELEJIR_POKE){
		if(jugador1->elejidos){
			system("clear");
			printf("Ya seleccionaste tus pokemones!\n");
		}
		else
			elejir_poke(juego, adversario, lista, jugador1);
	}
	else if(comando == ATACAR){
		if(!jugador1->elejidos){
			system("clear");
			printf("No seleccionaste tus pokemones todavia!\n");
		}
		else
			ataque(juego, adversario);
	}
	else if(comando == PUNTAJE)
		mostrar_puntaje(juego);
	else
		printf("Comando inexistente, preciona 'm' para más ayuda\n");
}

int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Error, no se especifico el archivo! Por favor ingrese: ./main 'nombre_archivo'\n");
		return -1;
	}

	juego_t *juego = juego_crear();

	if(!cargar_archivo(juego, argv[1]))
		return -1;

	lista_t * lista = juego_listar_pokemon(juego);

	adversario_t * adversario = adversario_crear(lista);
	struct data_jugador *jugador1 = calloc(1, sizeof( struct data_jugador));

	if(!adversario || !jugador1){
		printf("Error al iniciar el juego, intente nuevamente!");
		return false;
	}

	char comando = ' ';
	while (!juego_finalizado(juego))
	{

		printf("Insertar comando: ");
        scanf(" %c", &comando);

		comander(comando,lista, juego, adversario, jugador1);
	}

	free(jugador1->Pokemones[0]);
	free(jugador1->Pokemones[1]);
	free(jugador1->Pokemones[2]);
	free(jugador1->Pokemones);
	free(jugador1);

	system("clear");
	printf("EL ganador fue");

	juego_destruir(juego);

	return 0;
}
