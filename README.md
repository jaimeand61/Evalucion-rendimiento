# Taller de Evaluación de Rendimiento - Sistemas Operativos
**Pontificia Universidad Javeriana**  
**Fecha:** 26 de Abril 2026  
**Autor**
- Jaime Andrés Molina Villamarín

---

## Descripción General

Este proyecto implementa y compara dos algoritmos de multiplicación 
de matrices en lenguaje C, evaluando sus versiones en serie y en 
paralelo mediante hilos POSIX (pthreads) y procesos Fork, sobre 
tres plataformas diferentes con sistema operativo Linux.

### Algoritmos implementados
- **FxC (Por Columnas):** Algoritmo clásico de multiplicación accediendo la matriz B por columnas.
- **FxT (Por Transpuesta):** Algoritmo optimizado que transpone la matriz B antes de multiplicar, mejorando el aprovechamiento del caché.

### Modelos de paralelismo
- **Hilos POSIX (pthreads):** Memoria compartida entre hilos del mismo proceso.
- **Procesos Fork:** Cada proceso hijo opera sobre su propia copia del espacio de memoria.

---

## Estructura del Proyecto

```
evalRend2610/
├── Makefile              # Compilación de todos los ejecutables
├── lanzador.pl           # Script Perl de automatización de experimentos
├── src/
│   ├── moduloMM.h        # Cabecera con declaración de funciones
│   ├── moduloMM.c        # Implementación de funciones de soporte
│   ├── mxmPosixFxC.c     # Multiplicación con hilos POSIX - algoritmo FxC
│   ├── mxmPosixFxT.c     # Multiplicación con hilos POSIX - algoritmo FxT
│   ├── mxmForkFxC.c      # Multiplicación con procesos Fork - algoritmo FxC
│   └── mxmForkFxT.c      # Multiplicación con procesos Fork - algoritmo FxT
├── bin/                  # Binarios generados tras compilar
└── Soluciones/           # Archivos .dat generados por lanzador.pl
```

---

## Requisitos

- Sistema operativo **Linux** (nativo o máquina virtual)
- Compilador **GCC**
- **Perl** instalado (para ejecutar lanzador.pl)
- Librería **pthreads** (incluida en GCC con flag `-lpthread`)

---

## Compilación

Desde la carpeta raíz del proyecto ejecutar:

```bash
make All
```

Esto generará los siguientes binarios en la carpeta `bin/`:
- `mxmPosixFxC`
- `mxmPosixFxT`
- `mxmForkFxC`
- `mxmForkFxT`

---

## Uso de los ejecutables

Todos los ejecutables reciben dos argumentos:

```bash
./bin/<ejecutable> <tamMatriz> <numHilos/Procesos>
```

### Ejemplos:

```bash
# Multiplicación clásica con hilos POSIX, matriz 400x400, 4 hilos
./bin/mxmPosixFxC 400 4

# Multiplicación con transpuesta usando hilos POSIX, matriz 800x800, 8 hilos
./bin/mxmPosixFxT 800 8

# Multiplicación clásica con procesos Fork, matriz 1200x1200, 4 procesos
./bin/mxmForkFxC 1200 4

# Multiplicación con transpuesta usando Fork, matriz 400x400, 2 procesos
./bin/mxmForkFxT 400 2
```

La salida muestra el tiempo de ejecución en microsegundos.

---

## Automatización con lanzador.pl

El script `lanzador.pl` automatiza la ejecución de todos los experimentos. 
Antes de ejecutarlo, modificar las siguientes variables según la batería 
de experimentación deseada:

```perl
@Nombre_Ejecutable = ("mxmPosixFxC","mxmPosixFxT","mxmForkFxC","mxmForkFxT");
@Size_Matriz       = ("400","800","1200");  # Tamaños de matriz
@Num_Hilos         = (1,2,4,8);            # Número de hilos/procesos
$Repeticiones      = 30;                   # Repeticiones por escenario
$Resultados        = "Soluciones";         # Carpeta de salida
$Binarios          = "bin";                # Carpeta de binarios
```

### Ejecución del lanzador:

```bash
# Ejecución normal
./lanzador.pl

# Ejecución en segundo plano (recomendado para experimentos largos)
nohup ./lanzador.pl & exit
```

Los resultados se guardan en archivos `.dat` dentro de la carpeta 
`Soluciones/` con el formato:

```
<ejecutable>-<tamMatriz>-Hilos-<numHilos>.dat
```

Por ejemplo: `mxmPosixFxC-400-Hilos-4.dat`

---

## Plataformas utilizadas

| Sistema | Procesador | Hilos Lógicos | SO |
|---|---|---|---|
| S1 - VM Javeriana | Intel Xeon Gold 6348 | 4 vCPUs | Linux (VMware) |
| S2 - PC Libre | Intel Core i5-1235U | 12 hilos | Ubuntu 22.04 WSL2 |
| S3 - PC Taskset | Intel Core i5-1235U | 2 (restringido) | Ubuntu 22.04 WSL2 |

Para restringir los núcleos en S3 se utilizó:
```bash
taskset -c 0,1 ./bin/<ejecutable> <tamMatriz> <numHilos>
```

---

## Descripción de archivos fuente

### `moduloMM.h`
Cabecera con la declaración de todas las funciones compartidas entre 
los ejecutables. Define la interfaz modular del proyecto.

### `moduloMM.c`
Implementa las funciones de soporte:
- `iniMatrix()` — Inicializa matrices con valores aleatorios
- `impMatrix()` — Imprime la matriz si su dimensión es menor a 13
- `matrixTRP()` — Transpone la matriz B para optimizar acceso a caché
- `mxmForkFxC()` — Multiplicación por columnas para procesos Fork
- `mxmForkFxT()` — Multiplicación por transpuesta para procesos Fork
- `InicioMuestra()` — Captura tiempo inicial con gettimeofday
- `FinMuestra()` — Calcula e imprime el tiempo transcurrido

### `mxmPosixFxC.c`
Implementa la multiplicación clásica (FxC) usando hilos POSIX. 
Cada hilo calcula un rango de filas de la matriz resultado. 
Los hilos comparten las matrices globales `matrixA`, `matrixB` y `matrixC`.

### `mxmPosixFxT.c`
Implementa la multiplicación optimizada (FxT) usando hilos POSIX.
Transpone la matriz B antes de multiplicar para mejorar localidad 
de caché. Usa `matrixT` como matriz transpuesta compartida.

### `mxmForkFxC.c`
Implementa la multiplicación clásica (FxC) usando procesos Fork.
Cada proceso hijo calcula su segmento de filas de forma independiente
con su propia copia del espacio de memoria.

### `mxmForkFxT.c`
Implementa la multiplicación con transpuesta (FxT) usando procesos Fork.
Cada proceso hijo utiliza la matriz transpuesta `matT` para su cálculo,
mejorando el acceso secuencial a memoria.

### `lanzador.pl`
Script Perl que automatiza la ejecución de todos los experimentos, 
generando un archivo `.dat` por cada combinación de ejecutable, 
tamaño de matriz y número de hilos/procesos. Cada archivo contiene 
los tiempos de las N repeticiones configuradas.

---

## Métricas de evaluación

| Métrica | Fórmula | Descripción |
|---|---|---|
| Tiempo de Ejecución (T) | medido con `gettimeofday()` | Tiempo total en microsegundos |
| Speedup (S) | S = T1 / Tn | Factor de aceleración respecto al tiempo serial |
| Eficiencia (E) | E = S / n × 100% | Porcentaje de aprovechamiento de núcleos |

---

## Resultados obtenidos

Los experimentos se ejecutaron con las siguientes configuraciones:

| Parámetro | Valores | Justificación |
|---|---|---|
| Tamaños de matriz | 400, 800, 1200 | Evalúa comportamiento frente a jerarquía de memoria (L2, L3, RAM) |
| Hilos/Procesos | 1, 2, 4, 8 | División exacta de carga para arquitecturas multinúcleo |
| Repeticiones | 30 | Validez estadística según ley de grandes números |

Los archivos `.dat` con los resultados completos se encuentran en la carpeta `Soluciones/`.
