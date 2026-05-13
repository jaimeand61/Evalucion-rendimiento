/*#######################################################################################
#* Fecha: 26 de Abril 2026
#* Autor: J. Corredor , PhD
#* Nombre Estudiantes: Federico Restrepo Guzman, Jaime Andres Molina Villamarin, Nicholas Ruiz
#* Programa:
#*      Multiplicación de Matrices algoritmo clásico
#* Versión:
#*      Paralelismo con Hilos Pthreads "Posix" 
######################################################################################*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "moduloMM.h" 

pthread_mutex_t MM_mutex;
double *matrixA, *matrixB, *matrixT, *matrixC; 

struct parametros{
        int nH;
        int idH;
        int N;
};
/* Cada hilo calcula de forma independiente un segmento de la matriz resultante */
void *mxmPosixFxT(void *variables){
	struct parametros *data = (struct parametros *)variables;
	int idH		= data->idH;
	int nH		= data->nH;
	int D		= data->N;
	int filaI	= (D/nH)*idH;
	int filaF	= (D/nH)*(idH+1);

    for (int i = filaI; i < filaF; i++){
        for (int j = 0; j < D; j++){
			double *pA, *pB, Suma = 0.0;
			pA = matrixA + i*D; 
			pB = matrixT + j*D;
            for (int k = 0; k < D; k++, pA++, pB++){
				Suma += *pA * *pB;
			}
			matrixC[i*D+j] = Suma;
		}
	}

	pthread_mutex_lock (&MM_mutex);
	pthread_mutex_unlock (&MM_mutex);
	pthread_exit(NULL);
}
/* Se crean múltiples hilos de ejecución para repartir el trabajo de la matriz */
int main(int argc, char *argv[]){
    /* Validación de argumentos: El programa nececita  el tamaño de matriz y el número de hilos */
	if (argc < 3){
		printf("Ingreso de argumentos \n $./ejecutable tamMatriz numHilos\n");
		exit(0);	
	}

    /* Captura de parámetros desde la línea de comandos */
    int N 		= (int) atoi(argv[1]); // Dimensión de la matriz (N x N)
    int num_Th	= (int) atoi(argv[2]); // Cantidad de hilos a crear

    /* Declaración de identificadores de hilos y atributos de los mismos */
    pthread_t p[num_Th];
    pthread_attr_t atrMM;

    /* Asignación de memoria dinámica para las matrices usando calloc para inicializar en cero */
	matrixA  = (double *)calloc(N*N, sizeof(double));
	matrixB  = (double *)calloc(N*N, sizeof(double));
	matrixC  = (double *)calloc(N*N, sizeof(double));
	matrixT  = (double *)calloc(N*N, sizeof(double));

    /* Inicialización de matrices con valores aleatorios */
	iniMatrix(matrixA, matrixB, N);
	impMatrix(matrixA, N);
	impMatrix(matrixB, N);

    /* Captura del tiempo inicial */
	InicioMuestra();

    /* Transposición de la Matriz B para mejorar el acceso en caché */
	matrixTRP(N, matrixB, matrixT);

    /* Configuración de atributos  */
	pthread_mutex_init(&MM_mutex, NULL);
	pthread_attr_init(&atrMM);
	pthread_attr_setdetachstate(&atrMM, PTHREAD_CREATE_JOINABLE);

    /* Se distribuye la carga de trabajo equitativamente */
    for (int j=0; j<num_Th; j++){
		/* Reservación de memoria para los argumentos de cada hilo de forma independiente */
		struct parametros *datos = (struct parametros *) malloc(sizeof(struct parametros)); 
		datos->idH = j;      // Identificador único del hilo
		datos->nH  = num_Th; // Total de hilos
		datos->N   = N;      // Dimensión del problema

        /* Lanzamiento del hilo ejecutando la función mxmPosixFxT */
        pthread_create(&p[j],&atrMM,mxmPosixFxT,(void *)datos);
	}

    /* El programa principal espera a que todos los hilos terminen su cálculo */
    for (int j=0; j<num_Th; j++)
        pthread_join(p[j],NULL);

    /* Captura del tiempo final */
	FinMuestra();
	
    /* Impresión del resultado final */
	impMatrix(matrixC, N);

	/* Liberación de Memoria Dinámica */
	free(matrixA);
	free(matrixB);
	free(matrixC);
	free(matrixT);

    /* Limpieza de hilos */
	pthread_attr_destroy(&atrMM);
	pthread_mutex_destroy(&MM_mutex);
	pthread_exit (NULL);

	return 0;
}
