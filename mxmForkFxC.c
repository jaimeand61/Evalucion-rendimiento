/*#######################################################################################
#* Fecha: 26 de Abril, 2026
#* Autor: J. Corredor , PhD
#* Nombre Estudiantes: Federico Restrepo Guzman, Jaime Andres Molina Villamarin, Nicholas Ruiz
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Procesos Fork 
######################################################################################*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "moduloMM.h" 

int main(int argc, char *argv[]) {
 /* Validación de la entrada de argumentos desde la terminal */
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		exit(0);
	}
 /* Captura de parámetros: dimensión de la matriz y número de procesos */
	int N		= (int) atoi(argv[1]);
	int num_P  	= (int) atoi(argv[2]);
    int filasxP	= N/num_P;
 /* Reserva de memoria dinámica para las matrices */
	double *matA = (double *) calloc(N*N, sizeof(double));
	double *matB = (double *) calloc(N*N, sizeof(double));
	double *matC = (double *) calloc(N*N, sizeof(double));
 /* Inicialización y visualización inicial */
    iniMatrix(matA, matB, N);
    impMatrix(matA, N);
    impMatrix(matB, N);
 /* Inicio de la medición de tiempo para la evaluación de rendimiento */
	InicioMuestra();

	for (int i = 0; i < num_P; i++) {
		pid_t pid = fork();
        
        if (pid == 0) { 
 /* Código ejecutado exclusivamente por el proceso HIJO */
            int filaI = i*filasxP;
            int filaF = (i == num_P - 1) ? N : filaI + filasxP;
 /* Ejecución del algoritmo de multiplicación por columnas */
			mxmForkFxC(matA, matB, matC, N, filaI, filaF); 
            
			if(N<11){
           		printf("\nChild PID %d calculated rows %d to %d:\n", getpid(), filaI, filaF-1);
            	for (int f = filaI; f < filaF; f++) {
                	for (int c = 0; c < N; c++) {
                    	printf(" %.2f ", matC[N*f+c]);
                	}
                	printf("\n");
            	}
			}
            exit(0); 
        } else if (pid < 0) {
 /* Control de errores en caso de fallo en la creación del proceso */
            perror("fork failed");
            exit(1);
        }
    }
    
    for (int i = 0; i < num_P; i++) {
        wait(NULL);
    }
 /* Finalización de la medición de tiempo */
	FinMuestra(); 

 /* Liberación de memoria para evitar fugas de recursos */
	free(matA);
	free(matB);
	free(matC);

    return 0;
}
