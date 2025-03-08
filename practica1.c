#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <limits.h>

#define TAG_MESSAGE 1
#define INFINITY INT_MAX

void encontrarRutaCorta(int size, int distancia[size][size]){
    printf("Aplicamos Dijkstra para encontrar la ruta mas corta desde 0\n");

    int dist[size]; // guarda las distancias mas cortas desde 0
    bool sptSet[size]; // el nodo esta incluido en el camino mas corto

    // Inicializar todas las distancias como infinito y sptSet[] como falso
    for (int i = 0; i < size; i++) {
        dist[i] = INFINITY;
        sptSet[i] = false;
    }

    // La distancia desde el nodo de origen es 0
    dist[0] = 0;

    for (int count = 0; count < size - 1; count++) {

        int min = INFINITY, u;

        // Encuentra el vertice con la distancia minima
        for (int v = 0; v < size; v++)
            if (sptSet[v] == false && dist[v] <= min)
                min = dist[v], u = v;

        // marcamos el vertice como visitado
       sptSet[u] = true;

        // Actualizamos la distancia de los vertices adyacentes al vertice seleccionado
        for (int v = 0; v < size; v++)
            // si el vertice no esta en el camino mas corto, hay un camino de u a v y el peso de u a v es menor que el peso actual de v
            // entonces actualizamos el peso de v
            if (!sptSet[v] && distancia[u][v] && dist[u] != INFINITY && dist[u] + distancia[u][v] < dist[v])
                dist[v] = dist[u] + distancia[u][v];
    }

    // Imprimimos la distancia de los nodos desde el nodo 0
    printf("Vertice \t Distancia desde el nodo 0\n");
    for (int i = 0; i < size; i++)
        printf("%d \t\t %d\n", i, dist[i]);
}

int main(int argc, char** argv){
    // Incializar open MPI
    int size; // el numero de nodos de nuestro programa
    int rank;//id unico de cada procesador

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // variables que vamos usar para la comunicacion
    int * send_data = (int *)malloc(size * sizeof(int));
    int * recv_data = (int *)malloc(size * sizeof(int));
    MPI_Request* send_request = (MPI_Request*)malloc(size*sizeof(MPI_Request));
    MPI_Request* recv_request = (MPI_Request*)malloc(size*sizeof(MPI_Request));

    int (*retraso)[size] = malloc(size * sizeof(*retraso)); // matriz, nos da el peso, [0][2] nos va a dar el peso de 0 a 2

    // Inicializar las distancias o pesos
    for(int i=0; i<size; i++){
        for(int j= 0; j<size; j++){
            if(i == j){
                retraso[i][j] = 0;
            }else{
                retraso[i][j] = INFINITY;
            }
        }
    }

    // definimos la topologia y mandar retrasos aleatorios todos vs todos en un rango del 0 al 100
    srand(time(NULL)+rank);

    // con la topologia todos vs todos podemos enviar mensajes y recibir el retraso en comunicacion (peso) de todos los nodos
    for(int nodo=0; nodo < size; nodo++){
        if(nodo!= rank){
            int valor_aleatorio = (rand() % 1000) + 1;
            send_data[nodo] = valor_aleatorio;
            MPI_Isend(&send_data[nodo], 1, MPI_INT, nodo, TAG_MESSAGE, MPI_COMM_WORLD, &send_request[nodo]);
            MPI_Irecv(&recv_data[nodo], 1, MPI_INT, nodo, TAG_MESSAGE, MPI_COMM_WORLD, &recv_request[nodo]);
        }else{
            send_request[nodo] = MPI_REQUEST_NULL;
            recv_request[nodo] = MPI_REQUEST_NULL;
        }
    }

    // Esperar a que se reciban todos los nodos
    int received = 0;

    do{
        int nodo;
        MPI_Waitany(size, recv_request, &nodo, MPI_STATUS_IGNORE); // received == size -1
        received++;
    }while(received < size -1);

    // Update the retraso matrix with the received data
    for(int nodo=0; nodo < size; nodo++){
        if(nodo != rank){
            retraso[rank][nodo] = recv_data[nodo];
        }
    }

    // Gather all retraso matrices from all processes
    MPI_Allgather(MPI_IN_PLACE, size, MPI_INT, retraso, size, MPI_INT, MPI_COMM_WORLD);

	// Imprime los retrasos
	printf("Los retrasos son:\n");
	for(int i=0; i<size; i++){
		for(int j=0; j<size; j++){
			printf("%d ", retraso[i][j]);
		}
		printf("\n");
	}
	
    if(rank == 0){
        encontrarRutaCorta(size, retraso);
    }

    free(retraso);
    free(recv_request);
    free(send_request);
    free(recv_data);
    free(send_data);

    MPI_Finalize();

    return 0;
}
