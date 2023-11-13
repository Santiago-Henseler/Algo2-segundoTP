<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

## Enunciado

En el TP1 creamos un TDA que nos permite cargar archivos con pokemon y acceder a la información cargada. Ahora lo que queremos es implementar un juego utilizando pokemones.

Para implementar este TP se permite reutilizar cualquiera de los TDA implementados en la materia (listas, pilas, colas, abb, hash, heap).

### Reglas del juego

- El juego requiere de 2 jugadores. 
- Al principio del juego se le presenta a los jugadores un listado de los pokemon disponibles.
- Cada jugador selecciona 3 pokemon de los disponibles.
  - Los 3 pokemon deben ser diferentes, no se puede repetir pokemon
  - Ambos jugadores realizan su elección sin conocer la elección del otro jugador
- Una vez seleccionados los pokemon por ambos usuario, se informan las elecciones.
  - Los primeros 2 pokemon seleccionados por el usuario son pokemon propios mientras que el tercer pokemon debe ser asignado como tercer pokemon del otro jugador
  - Por ejemplo, el jugador 1 selecciona A, B y C; el jugador 2 selecciona D, E y F. El equipo final del jugador 1 está formado por A, B y F, mientras que el equipo de jugador 2 queda D, E y C.
  - Si bien cada jugador debe seleccionar 3 pokemon diferentes, es posible que en el equipo final queden pokemon duplicados (ya que el tercer pokemon es elegido secretamente por el otro jugador)
- El juego se desarrolla por rondas. Como cada jugador tiene 3 pokemon y cada pokemon tiene 3 ataques, la cantidad de rondas del juego siempre es 9.
  - En cada ronda cada jugador decide un pokemon y un ataque de ese pokemon.
  - Los pokemon seleccionados se enfrentan.
  - Se le asigna a cada jugador un puntaje que se calcula en base al poder del ataque y el tipo del ataque y del pokemon adversario.
  - Una vez utilizado un ataque de un pokemon, este no puede ser utilizado nunca mas durante el juego.
- Al finalizar el juego, gana el jugador con mas puntos.

### Puntaje

El puntaje se calcula en base al tipo del ataque y el tipo del pokemon que recibe el ataque. El tipo del pokemon atacante no influye en el puntaje. Las combinaciones posibles y la lógica esperada se encuentra documentada en `juego_jugar_turno`.

### Adversario

El juego requiere de dos jugadores para ser jugado. Por este motivo se incluye `adversario.h`. Este TDA debe implementar la lógica de un rival contra el cual se puede jugar. Tener en cuenta que muchas de las validaciones de juego también son realizadas por el adversario (saber qué combinaciones de pokemon/ataque ya fueron utilizados) por lo que es posible (aunque no necesario) poner funcionalidad en común en un nuevo archivo. Como siempre, los archivos `.h` no deben ser modificados.

## Puntos a desarrollar en el informe

- Explicar las estructuras utilizadas para implementar la lógica del juego y del adversario. Cuando utilice algún TDA de los implementados en la materia explique por qué.
- Explique las complejidades de cada función implementada.

