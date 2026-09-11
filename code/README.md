# Documentación

## Entrega

La entrega se realiza vía **aula.usm.cl** en formato `.zip`.

## Multiplicación de matrices

### Programa principal

Desde `code/matrix_multiplication`:

```bash
mingw32-make run INPUT_DIR=data/matrix_input OUTPUT_DIR=data/matrix_output MEASUREMENTS=data/measurements/a.txt
```

El programa lee pares `*_1.txt` y `*_2.txt`, ejecuta `naive` y `strassen`,
guarda una salida por algoritmo en `data/matrix_output` y registra tiempo y
memoria en `data/measurements/a.txt`.

### Scripts

Para generar los inputs definidos por el enunciado:

```bash
mingw32-make generate-inputs
```

Para generarlos y ejecutar inmediatamente ambos algoritmos:

```bash
mingw32-make experiment
```

## Ordenamiento de arreglo unidimensional

Algoritmos: MergeSort, QuickSort, PatienceSort, std::sort.

### Programa principal

Desde `code/sorting`:

```bash
mingw32-make run INPUT_DIR=data/array_input OUTPUT_DIR=data/array_output MEASUREMENTS=data/measurements/a.txt
```

El programa ejecuta los cuatro algoritmos, guarda una salida por algoritmo en
`data/array_output` y registra tiempo y memoria en `data/measurements/a.txt`.

### Scripts

Para generar los inputs definidos por el enunciado:

```bash
mingw32-make generate-inputs
```

Para generarlos y ejecutar inmediatamente los cuatro algoritmos:

```bash
mingw32-make experiment
```
