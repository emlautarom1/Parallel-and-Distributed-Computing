# Diferencias

En la versión 'a', el for-loop se ejecuta en todos los threads sin distinción alguna, por lo que se imprimirá por cada thread 10 veces el mensaje "Hola mundo (tid-del-thread)".

Por otra parte, en la versión 'b', el for-loop se distribuye en los threads (directiva 'omp-for') de tal manera que solo se ejecutan 10 iteraciones del loop entre todos los threads disponibles, es decir, solo se imprime 10 veces el mensaje "Hola mundo (tid-del-thread)", sin importar la cantidad de threads que se utilicen.

# Variable 'n' compartida

En el caso de la versión 'a' la cantidad de veces que se imprime por pantalla el mensaje es indefinido: todos los threads comparten la misma variable 'n' por lo que todos escriben y leen la misma posición de memoria al ejecutar el loop. De esta manera un thread puede imprimir 't1' veces el mensaje, otro 't2' veces, y la suma de [t1..tn] puede ser mayor que 'n'. En otras palabras, el mensaje puede (y seguramente así suceda) imprimirse más de 'n' veces

Por otra parte, en la versión 'b' no hay diferencia si la variable 'n' es o no privada ya que 'OpenMP' se encarga de distribuir el for-loop entre los threads disponibles, es decir, se sigue imprimiendo 10 veces por consola el mensaje "Hola mundo (tid-del-thread)", sin importar la cantidad de threads que se utilicen.