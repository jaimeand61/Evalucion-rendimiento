/*#######################################################################################
 #* Fecha: 26 de Abril 2026
 #* Autor: J. Corredor , PhD
 #* Nombre Estudiantes: Federico Restrepo Guzman, Jaime Andres Molina Villamarin, Nicholas Ruiz
 #* Modulo: 
 #       - Implementación de utilidades para Multiplicación de Matrices
 #* Versión:
 #	 	Concurrencia de Tareas: Paralelismo sobre Multiplicación de Matrices
 #* Descripción:
 #       - Este módulo contiene las funciones de soporte para la inicialización,
 #  		impresión, transposición y algoritmos de cálculo
######################################################################################*/

#ifndef __MODULOMM_H__
#define __MODULOMM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include "moduloMM.h"

struct timeval inicio, fin;
/* Imprime la Matriz */
void impMatrix(double *matrix, int D){
	if(D < 13){
		printf("\n");
		for(int i=0; i<D*D; i++){
			if(i%D==0) printf("\n");
			printf("%.2f ", matrix[i]);
		}
		printf("\n**-----------------------------**\n");
	}
}
/* Realiza el intercambio de índices i,j a j,i */
void matrixTRP(int N, double *mB, double *mT){
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			mT[i*N+j] = mB[j*N+i];
	impMatrix(mT, N);
}
/* Multiplicación de matrices optimizada para procesos (Fork) usando la matriz transpuesta*/
void mxmForkFxT(double *mA, double *mT, double *mC, int D, int filaI, int filaF){
    for (int i = filaI; i < filaF; i++)
        for (int j = 0; j < D; j++) {
			double *pA, *pB, Suma = 0.0;
			pA = mA+i*D;
			pB = mT+j*D;
            for (int k = 0; k < D; k++, pA++, pB++) 
				Suma += *pA * *pB;	
			mC[i*D+j] = Suma;
        }
}
/* Multiplicación de matrices estándar por columnas para procesos (Fork) */
void mxmForkFxC(double *mA, double *mB, double *mC, int D, int filaI, int filaF){
    for (int i = filaI; i < filaF; i++)
        for (int j = 0; j < D; j++) {
			double *pA, *pB, Suma = 0.0;
			pA = mA+i*D;
			pB = mB+j;
            for (int k = 0; k < D; k++, pA++, pB+=D) 
				Suma += *pA * *pB;	
			mC[i*D+j] = Suma;
        }
}
/*Se usa gettimeofday para medir el tiempo real que tarda el programa en ejecutarse*/
void InicioMuestra(){
	gettimeofday(&inicio, (void *)0);
}
/*Se usa gettimeofday para medir el tiempo real que tarda el programa en ejecutarse */
void FinMuestra(){
	gettimeofday(&fin, (void *)0);
	fin.tv_usec -= inicio.tv_usec;
	fin.tv_sec  -= inicio.tv_sec;
	double tiempo = (double) (fin.tv_sec*1000000 + fin.tv_usec); 
	printf("%9.0f \n", tiempo);
}

/* se inicializa la matriz */
void iniMatrix(double *m1, double *m2, int D){
	srand(time(NULL));
	for(int i=0; i<D*D; i++, m1++, m2++){
		*m1 = (double)rand()/RAND_MAX*(5.0-1.0);	
		*m2 = (double)rand()/RAND_MAX*(9.0-2.0);	
	}
}

#endif
