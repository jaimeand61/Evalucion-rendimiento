/*#######################################################################################
 #* Fecha:26 de Abril 2026 
 #* Autor: J. Corredor , PhD
 #* Nombre Estudiantes: Federico Restrepo Guzman, Jaime Andres Molina Villamarin, Nicholas Ruiz
 #* Modulo:
 #       -	Interfaz de funciones para Evaluación de Rendimiento
 #* Versión:
 #*	 	Concurrencia de Tareas: Paralelismo sobre Multiplicación de Matrices
 #* Descripción:
 #       - Define la interfaz modular para la multiplicación de matrices
 #		Garantiza la abstracción entre los algoritmos de paralelismo (Hilos/Procesos)
######################################################################################*/

#ifndef __MODULOMM_H__
#define __MODULOMM_H__

/* Inicializa dos matrices m1 y m2 de dimensión D con valores aleatorios. */
void iniMatrix(double *m1, double *m2, int D);

/* Captura la marca de tiempo inicial  */
void InicioMuestra();

/* Captura la marca de tiempo final y calcula la diferencia . */
void FinMuestra();

/* Imprime la matriz en consola si su dimensión D es pequeña (D < 13). */
void impMatrix(double *matrix, int D);

/* Realiza la transposición de la matriz mB y la almacena en mT. 
   Optimiza el acceso a memoria para evitar fallos de caché. [cite: 20] */
void matrixTRP(int N, double *mB, double *mT);

/* Algoritmo de multiplicación por columnas para el modelo de Procesos (Fork). */
void mxmForkFxC(double *mA, double *mB, double *mC, int D, int filaI, int filaF);

/* Algoritmo de multiplicación usando matriz transpuesta para el modelo de Procesos (Fork). */
void mxmForkFxT(double *mA, double *mT, double *mC, int D, int filaI, int filaF);

#endif
