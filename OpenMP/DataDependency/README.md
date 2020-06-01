# Ejercicio 3

En todos los casos es posible paralelizar los loops. Vease el código de ejemplo en `src/main.c`

# Ejercicio 4

## Caso 1

```c++
    for (int i = 0; i < n; i++) {
        a[i] = 2.3 * i;
        if (a[i] < b[i]) break;
    }
```

El código no es paralelizable porque existe un posible salto desde el loop (salto fuera de la región paralela) en la forma de `if (a[i] < b[i]) break;` con la instrucción `break`. El compilador rechaza este código con el siguiente mensaje: `error: break statement used with OpenMP for loop`


## Caso 2

```c++
    flag = 0;
    for (int i=0; (i<n) & (!flag); i++){
        a[i] = 2.3 * i;
        if (a[i] < b[i]) flag = 1;
    }
```

Nuevamente, el código no es paralelizable porque existe un posible salto desde el loop (salto fuera de la región paralela) en la forma de `if (a[i] < b[i]) flag = 1;` dado a que `flag` es una variable utilizada como predicado de control en el loop. El compilador rechaza este código con el siguiente mensaje: `error: invalid controlling predicate`