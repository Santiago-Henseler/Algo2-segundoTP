<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Santiago Henseler - 110732 - shenseler@fi.uba.ar

- Para compilar:

```bash
make main
```

- Para ejecutar:

```bash
./main nombre_archivo.txt
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```
valgrind ./main nombre_archivo.txt
---
##  T.D.A. utilizados

- El primer T.D.A. que utilize para mi tp fue lista, porque fue una manera fácil de almacenar los pokemones cargados en el archivo, ya que insertar es una operación `o(1)`. Igualmente no es el mejor T.D.A. en cuestion de complejidad para buscar elementos, la complejidad de buscar en la lista es `o(n)`. Yo hubiera utilizado el T.D.A. hash para poder almacenar los pokemones, porque la complejidad de la busqueda es casi `o(1)`. Pero como pocas veces se llama a la operación buscar y en las mismas consignas te pedia agregar una lista al juego, opte por usarlo.

- El segundo T.D.A. que agrege al tp fue abb, para almacenar los pokemones seleccionados por los jugadores. Primero habia optado por utilizar el T.D.A. hash por su rapidez al momento de buscar claves, pero me surgio un problema. El T.D.A. hash no admite claves repetidas, entonces en las situaciones donde el adversario te elegia un pokemon que ya habias elegido el juego rompia. Para solucionarlo use abb, a pesar de que la complejidad de busqueda es `o(log(n))` en el mejor caso(igualmente sigue siendo buena), me permitia usar pokemones repetidos. 


### Logica del juego

Una vez iniciado el juego y pasado un archivo valido por parametro, se abre un menu con distintos comandos. 

- (e) Elejir pokemones: con este comando te da la opción de elegir los pokemones cargados en el archivo.
    
    Una vez indicado por consola los pokemones que queres elegir el juego llama a la función `juego_seleccionar_pokemon()` la cual primero valida que los pokemones existan y no sean repetidos, una vez cumpla las validaciones invoca la función `jugador_cargar_pokes()` la cual se encarga de almacenar los pokemones y las jugadas validas a cada jugador.

Implemente la estructura pública `jugador_t` donde almacenó el puntaje, los Pokemones elegidos y los movimientos posibles en un abb. Cómo en el juego se repite varias veces esta estructura y la operación de cargar Pokemones lo modularize en el archivo `jugador.h`.

```c
    // busca el puntero al pokemon elejido
    pokemon_t *poke = lista_buscar_elemento(lista, comparador_poke,(void*) nombre); --> 0(n)

    // almacena los ataques del pokemon elejido
    con_cada_ataque(poke, buscar_ataques, (void *)&almacenador); --> o(n)

    // guarda las posibles jugadas en el abb
    abb_insertar(jugador->movimientos_posibles, (void*)jugada_valida); --> o(1)
```

Haciendo el analisis de complejidad y contando todas las instrucciones se llega a la siguiente ecuación: `T(n) = o(n) + o(n) + 3*o(1)` . Entonces los algoritmos `jugador_cargar_pokes()` tiene una complejidad de O(n).

Para poder almacenar los ataques de los pokémones implemente la estructura pública `almacenador_t`. Donde con ayuda de la función `almacenar_ataques()` y con el tp1 obtengo todos los ataques de un pokémon y los guardo en el almacenador.

Habiendo almacenado bien los pokemones seleccionados el juego selecciona los pokemones del adversario llamando a la funcion `adversario_seleccionar_pokemon()` 

```c
	// Busca en la lista de pokemones aleatoreamente 3 pokemones
	pokemon_t *poke1 = lista_elemento_en_posicion(adversario->lista_poke, (size_t) rand() % tamanio); -->o(n)
	pokemon_t *poke2 = lista_elemento_en_posicion(adversario->lista_poke, (size_t) rand() % tamanio); -->o(n)
	pokemon_t *poke3 = lista_elemento_en_posicion(adversario->lista_poke, (size_t) rand() % tamanio); -->o(n)

	// Si se repiten los pokemones vuelve a llamar a la funcion, hasta que los pokemones sean diferentes
	if(strcmp(*nombre1, *nombre2) == 0){
		return adversario_seleccionar_pokemon(adversario, nombre1, nombre2, nombre3);
	}

```

Por ultimo se invoca a `adversario_pokemon_seleccionado()` la cual notifica al adversario los pokemones elegidos y que el jugador le eligio, almacenandolos en una estructura de `jugador_t`.

- (a) Realizar ataque: usando este comando te da la opción de realizar un ataque eligiendo entre tus pokemones

    Primero inicializa la jugada del adversario con la funcón `adversario_proxima_jugada` la cual recorre el arbol donde esta almacenadas las jugadas posibles, seleccionando una aleatoreamente y luego quitandola del abb.  

```c
    // Guarda todas las jugadas validas en la variable jugadas_validas
	abb_recorrer(adversario->jugador->movimientos_posibles, INORDEN, (void **)jugadas_validas, (size_t)cantidad); --> o(n)

    // Selecciona una jugada aleatoreamente
	char * jugada = jugadas_validas[rand() % cantidad]; --> o(1)

    // Saca la jugada elegida del abb
	void * jugada_anterior = abb_quitar(adversario->jugador->movimientos_posibles, (void*)jugada); --> o(log(n))
```
Analizando la complejidad de `adversario_proxima_jugada` se llega a la siguiente ecuación: `T(n) = o(n) + o(log(n)) + o(1)`. Entonces concluimos que la complejidad es O(n). 

Una vez inicializado la jugada del adversario, el jugador va a cargar por consola con que pokemon elije y que ataque quiere hacer. Se corrobora que el ataque este bien llamando a la funcion `juego_jugar_turno()`.

```c
	// Valida si es una jugada valida y no se hizo antes
	if(!abb_buscar(juego->j1->movimientos_posibles, (void*)jugada1) || !abb_buscar(juego->j2->movimientos_posibles, (void*)jugada2)) --> 2*o(log(n))
		return resultado;

	// Busca la información de cada pokemon
	pokemon_t *poke1 = pokemon_buscar(juego->ip, jugada_jugador1.pokemon);  --> o(n)
	pokemon_t *poke2 = pokemon_buscar(juego->ip, jugada_jugador2.pokemon);  --> o(n)

	// Busca el ataque de cada pokemon 
	const struct ataque * poke1_ataque = pokemon_buscar_ataque(poke1, jugada_jugador1.ataque);  --> o(n)
	const struct ataque * poke2_ataque = pokemon_buscar_ataque(poke2, jugada_jugador2.ataque);  --> o(n)

	// Clasifica y puntea los ataques
	resultado.jugador1 = clasificar_ataque(poke1_ataque->tipo, poke2_ataque->tipo); --> o(1)
	resultado.jugador2 = clasificar_ataque(poke2_ataque->tipo, poke1_ataque->tipo); --> o(1)

	juego->j1->puntaje += puntear_ataque(resultado.jugador1, poke1_ataque); --> o(1)
	juego->j2->puntaje += puntear_ataque(resultado.jugador2, poke2_ataque); --> o(1)
```
Contando todas las instrucciones se llega a la siguiente ecuación: `T(n) = 4*o(n)+ 2*o(log(n)) + 4*o(1)` . Entonces los algoritmos `juego_jugar_turno()` tiene una complejidad de O(n).

Si los pokemones y ataques elegidos son correctos se muestra por pantalla el resultado del ataque con la funcion `mostrar_jugada()`
