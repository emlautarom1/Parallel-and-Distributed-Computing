# Planificación de Bucles

La diferencia principal entre los dos códigos (`main-a` y `main-b`) radica en los diferentes modos de planificación de los loops: `A` utiliza planificación estática mientras que `B` utiliza planificación dinámica, ambos con 4 iteraciones por thread.

## Caso A

En la práctica esto significa que para el caso de `A` cada thread ejecutará 4 iteraciones del loop de manera ordenada, es decir, el thread N ejecutará las iteraciones:
  - N .. N + 3,
  - N + NUM_THREADS * 4 .. N + NUM_THREADS * 4 + 3,
  - N + (NUM_THREADS * 4) * 2 .. N (NUM_THREADS * 4) * 2 + 3,
  - etc ...
Suponiendo que se cuenta con 4 threads, entonces el thread 0 ejecutará:
  - 0 .. 3
  - 16 .. 19
  - 32 .. 35
  - etc...
De esta manera, todos los threads tienen la misma carga y la planificación se puede realiazr por adelantado.

## Caso B

Por otra parte, ara el caso de `B`, cada thread ejecutará 4 iteraciones del loop pero no conoce previamente que iteraciones deberán realizar. Luego de terminar sus 4 iteraciones, cada thread deberá evaluar cuales son las iteraciones pendientes, las cuales pasará a procesar a continuación. De esta manera se logra "balancear" la carga en cada thread cuando las tareas dentro del loop son desiguales en cuanto a costo acorde al número de iteración. Notese que diferentes ejecuciones darán distintos resultados, a diferencia de `A`.

Este tipo de planificación es mas costoso dado a que existe una comunicación entre los threads para ponerse de acuerdo en como continuar el loop. Queda como responsabilidad del programador determinar los costos y beneficios de esta técnica