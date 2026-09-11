# Documentación

## Importante 
La tarea se realizo en windous.

## Entrega

La entrega se realiza vía **aula.usm.cl** en formato `.zip`.

## Multiplicación de matrices
Algoritmos: Strassen y Naive.

### Programa principal

Para que los comandos funcionen deben situarse en el directorio `code/matrix_multiplication`:

```Power Shell
make experimento
```

El programa creará las entradas y luego ejecutará cada una con los distintos algoritmos,
realizará las pruebas y generará las salidas, además de registrar las mediciones en el archivo `a.csv`.

### Scripts

Para generar los inputs definidos por el enunciado:

```bash
mingw32-make generar-inputs
```

Para generar los gráficos a partir del experimento realizado:

```bash
mingw32-make generar_plots
```


## Ordenamiento de arreglo unidimensional

Algoritmos: Merge Sort, Quick Sort, Patience Sort y `std::sort`.

### Programa principal

Para que los comandos funcionen deben situarse en el directorio `code/sorting`:

```Power Shell
mingw32-make experimento
```

El programa creará las entradas y luego ejecutará cada una con los distintos algoritmos,
realizará las pruebas y generará las salidas, además de registrar las mediciones en el archivo `a.csv`.

### Scripts

Para generar los inputs definidos por el enunciado:

```Power Shell
mingw32-make generar-inputs
```

Para generar los gráficos a partir del experimento realizado:

```Power Shell
mingw32-make generar_plots
```


## Modificaciones
Cabe mencionar que el algoritmo Quick Sort presentaba problemas de rendimiento con entradas grandes:
la prueba no terminaba debido a la forma en que escogía el pivote. Por esta razón, se optó por
utilizar un pivote de "mediana de tres" y una partición de tres vías, que previene la degradación
con valores repetidos. Además, se redujo el uso de memoria en la pila al procesar primero la
partición más pequeña. Esto último fue importante porque, al intentar ejecutar el experimento,
la pila de ejecución se llenaba con arreglos muy grandes.

## Links    
Aquí se encuentran los enlaces y las referencias de los algoritmos utilizados para ordenar arreglos
y multiplicar matrices.

Merge Sort y Quick Sort:

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to Algorithms (3rd ed.), MIT Press. Cap. 2 (Merge Sort) y Cap. 7 (Quicksort).
https://en.wikipedia.org/wiki/Merge_sort
https://en.wikipedia.org/wiki/Quicksort

Patience Sort:

Mallows, C. L. (1963). "Patience Sorting". Bell System Technical Journal.
https://en.wikipedia.org/wiki/Patience_sorting

Naive y Strassen:

Strassen, V. (1969). "Gaussian elimination is not optimal". Numerische Mathematik, 13(4), 354-356.
Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to Algorithms (3rd ed.), MIT Press. Sección 4.2.
https://en.wikipedia.org/wiki/Strassen_algorithm
https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm 