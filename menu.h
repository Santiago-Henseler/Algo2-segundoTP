#ifndef MENU_H_
#define MENU_H_

#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "src/pokemon.h"
#include "src/ataque.h"
#include "src/juego.h"
#include "src/juego_aux.h"
#include "src/lista.h"
#include "src/adversario.h"
#include <stdio.h>
#include <stdlib.h>

#define MOSTRAR_MENU 'm'
#define MOSTRAR_POKE 's'
#define ELEJIDOS 'p'
#define ELEJIR_POKE 'e'
#define ATACAR 'a'
#define PUNTAJE 'v'

typedef struct menu menu_t;

menu_t * menu_crear(lista_t * lista);

void comander(char comando, juego_t * juego, adversario_t *adversario, struct menu *menu);

void mostrar_comandos();

void destruir_menu(lista_t *lista);

void mostrar_puntaje(juego_t * juego);

#endif