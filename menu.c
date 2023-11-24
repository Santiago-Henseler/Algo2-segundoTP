#include "menu.h"

struct menu
{
	char * pokemones[3];
	bool elejidos;
	lista_t *lista;
};

void mostrar_ataques(char *nombre,  lista_t* lista){

	almacenador_t * ataques = almacenar_ataques(nombre, lista);

	if(!ataques)
		return;

	printf("║  Ataques:   %s   -  %s   -  %s  ║\n", (char*)ataques->elemento[0], (char*)ataques->elemento[1], (char*)ataques->elemento[2]);

	free(ataques->elemento[0]);
	free(ataques->elemento[1]);
	free(ataques->elemento[2]);
	free(ataques->elemento);
}

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

void mostrar_seleccionados(struct menu *menu){
	system("clear");

	if(!menu->elejidos){
		printf("Todavia no seleccionaste ningun pokemon!\n");
		return;
	}

	printf("╔═══════════════════◓═════════════════════╗\n");
	printf("║             TUS POKEMONES               ║\n");
	printf("║                                         ║\n");
	printf("║                %s                       ║\n", menu->pokemones[0]);
	printf("║                                         ║\n");
	mostrar_ataques(menu->pokemones[0], menu->lista);

	printf("║                                         ║\n"
		   "║                %s                       ║\n", menu->pokemones[1]);
	printf("║                                         ║\n");
	mostrar_ataques(menu->pokemones[1], menu->lista);

	printf("║                                         ║\n"
	       "║                %s                       ║\n", menu->pokemones[2]);
	printf("║                                         ║\n");  
	mostrar_ataques(menu->pokemones[2], menu->lista);   
		   
	printf("║                                         ║\n"
		   "╚═══════════════════◓═════════════════════╝\n\n");

}

char * rta(RESULTADO_ATAQUE resultado){
	if(resultado == ATAQUE_EFECTIVO)
		return "efectivo";
	if(resultado == ATAQUE_INEFECTIVO)
		return "inefectivo";
	if(resultado == ATAQUE_REGULAR)
		return "regular";
	return "";
}

void mostrar_jugada(jugada_t adversario, jugada_t jugador, resultado_jugada_t resultado){
	system("clear");

	printf("╔═══════════════════◓═════════════════════╗\n");
	printf("║           RESULTADO ATAQUE              ║\n");
	printf("║                                         ║\n");
	printf("║  El rival uso:  %s con %s               ║\n", adversario.pokemon, adversario.ataque);
	printf("║                                         ║\n");
	printf("║    	 Y resulto  %s                    ║\n", rta(resultado.jugador2));
	printf("║                                         ║\n");
	printf("║   Usaste:  %s con %s                    ║\n", jugador.pokemon, jugador.ataque);
	printf("║                                         ║\n");
	printf("║    	     Y resulto  %s                ║\n", rta(resultado.jugador1));
	printf("║                                         ║\n");
	printf("║                                         ║\n"
		   "╚═══════════════════◓═════════════════════╝\n\n");

}

void ataque(juego_t * juego, adversario_t *adversario, struct menu *menu){
	jugada_t jugada_adversario = adversario_proxima_jugada(adversario);

	bool elejido = false;

	char *poke, *ataque;
	resultado_jugada_t resultado;
	jugada_t jugada_local;

	while (!elejido)
	{
		mostrar_seleccionados(menu);
		printf("Indicando el pokemon y el ataque de esta manera: 'nombre,ataque'\n\n");

		printf("Seleccionar pokemon: ");

		char linea[200];
		scanf("%s", linea);
		poke = strtok(linea, ",");
		ataque = strtok(NULL, ",");
		
		if(strcmp(poke, menu->pokemones[0]) != 0 && strcmp(poke, menu->pokemones[1]) != 0 && strcmp(poke, menu->pokemones[2]) != 0 )
			continue;		

		strcpy(jugada_local.pokemon, poke);
		strcpy(jugada_local.ataque, ataque);

		resultado = juego_jugar_turno(juego, jugada_local, jugada_adversario);
		
		if(resultado.jugador1 != ATAQUE_ERROR && resultado.jugador2 != ATAQUE_ERROR)
			elejido = true;
	}

	mostrar_jugada(jugada_adversario, jugada_local, resultado);
}

void elejir_poke(juego_t * juego, adversario_t *adversario, struct menu *menu){

	system("clear");
	printf("Para iniciar el juego selecciona 3 de los pokemones disponibles\n\n");
	mostrar_todos_pokes(menu->lista);
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

	char *nombre1, *nombre2, *nombre3;

	if(!adversario_seleccionar_pokemon(adversario, &nombre1, &nombre2, &nombre3)){
        system("clear");
		printf("Error al cargar pokemones, intentelo nuevamente!\n");
		return;
    }

    juego_seleccionar_pokemon(juego, JUGADOR2, nombre1, nombre2, nombre3);

	menu->pokemones[0] = calloc(1, sizeof(char)*strlen(elejido1)+1);
	menu->pokemones[1] = calloc(1, sizeof(char)*strlen(elejido2)+1);
	menu->pokemones[2] = calloc(1, sizeof(char)*strlen(nombre3)+1);

    adversario_pokemon_seleccionado(adversario, nombre1, nombre2, elejido3);

	strcpy(menu->pokemones[0], elejido1);
	strcpy(menu->pokemones[1], elejido2);
	strcpy(menu->pokemones[2], nombre3);

	if(!menu->pokemones[2] || !menu->pokemones[1] || !menu->pokemones[0]){
		system("clear");
		printf("Error al cargar pokemones, intentelo nuevamente!\n");
		return;
	}

    menu->elejidos = true;
 
	mostrar_seleccionados(menu);
}

void comander(char comando, juego_t * juego, adversario_t *adversario, struct menu *menu){

	comando = (char)tolower(comando);

	if(comando == MOSTRAR_MENU)
		mostrar_comandos();
	else if(comando == MOSTRAR_POKE){
		system("clear");
		mostrar_todos_pokes(menu->lista);
	}
	else if(comando == ELEJIDOS)
		mostrar_seleccionados(menu);
	else if(comando == ELEJIR_POKE){
		if(menu->elejidos){
			system("clear");
			printf("Ya seleccionaste tus pokemones!\n");
		}
		else
			elejir_poke(juego, adversario,menu);
	}
	else if(comando == ATACAR){
		if(!menu->elejidos){
			system("clear");
			printf("No seleccionaste tus pokemones todavia!\n");
		}
		else
			ataque(juego, adversario, menu);
	}
	else if(comando == PUNTAJE)
		mostrar_puntaje(juego);
	else
		printf("Comando inexistente, preciona 'm' para más ayuda\n");
}

menu_t * menu_crear(lista_t * lista){

    struct menu * nuevo_menu = calloc(1, sizeof(struct menu));

    if(!nuevo_menu)
        return NULL;

    nuevo_menu->lista = lista;

    return nuevo_menu;
}

void destruir_menu(menu_t *menu){
	lista_destruir(menu->lista);
	free(menu->pokemones[0]);
	free(menu->pokemones[1]);
	free(menu->pokemones[2]);
	free(menu);
}