#include "menu.h"

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

	menu_t * menu = menu_crear(lista);

	adversario_t * adversario = adversario_crear(lista);
	
	if(!adversario || !menu || !lista){
		printf("Error al iniciar el juego, intente nuevamente!");
		return false;
	}

	mostrar_comandos();

	char comando = ' ';
	while (!juego_finalizado(juego))
	{

		printf("Insertar comando: ");
        scanf(" %c", &comando);

		comander(comando, juego, adversario, menu);
	}

	system("clear");
	mostrar_puntaje(juego);
	destruir_menu(menu);
	juego_destruir(juego);
	adversario_destruir(adversario);

	return 0;
}
