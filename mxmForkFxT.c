/*#######################################################################################
#* Fecha: 26 de Abril 2026
#* Autor: J. Corredor , PhD
#* Nombre Estudiantes: Federico Restrepo Guzman, Jaime Andres Molina Villamarin, Nicholas Ruiz
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Procesos Fork 
#* Descripción: 
#*	Implementa la multiplicación de matrices utilizando la transpuesta
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "moduloMM.h" 

int main(int argc, char *argv[]) {
    
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		exit(0);
	}

    /* Definición de variables principales */
	int N		= (int) atoi(argv[1]); // Dimensión de la matriz
	int num_P 	= (int) atoi(argv[2]); // Número de procesos hijos
    int filasxP	= N/num_P;             // Filas asignadas por proceso

    /* Reserva de memoria dinámica para las matrices originales, transpuesta y resultado */
	double *matA = (double *) calloc(N*N, sizeof(double));
	double *matB = (double *) calloc(N*N, sizeof(double));
	double *matC = (double *) calloc(N*N, sizeof(double));
	double *matT = (double *) calloc(N*N, sizeof(double));

    /* Inicialización de las matrices A y B con valores aleatorios */
    iniMatrix(matA, matB, N);
    impMatrix(matA, N);
    impMatrix(matB, N);

    /* Inicio de toma de tiempo */
	InicioMuestra();
	
    /* Generación de la matriz transpuesta */
	matrixTRP(N, matB, matT);

    /* Bucle de creación de procesos paralelos */
	for (int i = 0; i < num_P; i++) {
		pid_t pid = fork();
        
        if (pid == 0) { 
            /* Segmento ejecutado por el proceso hijo */
            int filaI = i*filasxP;
            /* Manejo de residuos: el último proceso toma las filas sobrantes */
            int filaF = (i == num_P - 1) ? N : filaI + filasxP;
            
            /* Cálculo de la multiplicación utilizando la matriz transpuesta */
			mxmForkFxT(matA, matT, matC, N, filaI, filaF); 
            
            
			if(N<11){
           		printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), filaI, filaF-1);
            	for (int f = filaI; f < filaF; f++) {
                	for (int c = 0; c < N; c++) {
                    	printf(" %.2f ", matC[N*f+c]);
                	}
                	printf("\n");
            	}
			}
            /* Finalización segura del proceso hijo */
            exit(0); 
        } else if (pid < 0) {
            /* Gestión de errores si el sistema no puede crear más procesos */
            perror("fork failed");
            exit(1);
        }
    }
    
    /* El proceso padre espera la finalización de todos sus hijos */
    for (int i = 0; i < num_P; i++) {
        wait(NULL);
    }
 	
    /* Captura del tiempo final */
	FinMuestra(); 

	/* Liberación de memoria dinámica */
	free(matA);
	free(matB);
	free(matC);
	free(matT);

    return 0;
}