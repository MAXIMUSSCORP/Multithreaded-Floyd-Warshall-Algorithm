#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <limits.h>
#include <time.h>

#define INF 99999
#define MAX_THREADS 10000

sem_t readCountLock;
sem_t dbLock;
int readCount = 0;

typedef struct {
    int k;
    int thread_id;
    int V;
    int **dist;
} thread_data;

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void* writerThreads(void* arg) {
    thread_data* data = (thread_data*)arg;
    int k = data->k, i = data->thread_id, V = data->V;
    int **dist = data->dist;

    for (int j = 0; j < V; j++) {
        sem_wait(&dbLock); // Lock for writing
        dist[i][j] = MIN((dist[i][k] + dist[k][j]),(dist[i][j]));
        sem_post(&dbLock); // Unlock after writing
    }

    return NULL;
}

void* readerThreads(void* arg) {
    thread_data* data = (thread_data*)arg;
    int i = data->thread_id, V = data->V;
    int **dist = data->dist;

    for (int j = 0; j < V; j++) {
        sem_wait(&readCountLock);
        readCount++;
        if (readCount == 1) sem_wait(&dbLock);
        sem_post(&readCountLock);

        sem_wait(&readCountLock);
        readCount--;
        if (readCount == 0) sem_post(&dbLock);
        sem_post(&readCountLock);
    }

    return NULL;
}

void floydWarshall(int **dist, int V) {
    pthread_t threads[MAX_THREADS];
    thread_data data[MAX_THREADS];

    // Example adjustment: Only create writer threads as part of the algorithm
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            data[i].k = k;
            data[i].thread_id = i;
            data[i].V = V;
            data[i].dist = dist;
            pthread_create(&threads[i], NULL, writerThreads, (void *)&data[i]);
        }
        for (int i = 0; i < V; i++) {
            pthread_join(threads[i], NULL);
        }
        
        // After updating dist for all pairs with current k as intermediate, spawn reader threads
        for (int i = 0; i < V; i++) {
            pthread_create(&threads[i], NULL, readerThreads, (void *)&data[i]);
        }
        for (int i = 0; i < V; i++) {
            pthread_join(threads[i], NULL);
        }
    }
}


void printSolution(int **dist, int V) {
    printf("The shortest distances between every pair of vertices:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF)
                printf("%7s", "INF");
            else
                printf("%7d", dist[i][j]);
        }
        printf("\n");
    }
}

int main() {
    srand(time(NULL));

    int V, E;
    printf("Enter the number of vertices: ");
    scanf("%d", &V);

    E = (V * (V - 1)) / 4; // For simplicity, adjust this formula as needed

    sem_init(&dbLock, 0, 1); // Initialize semaphore for writers
    sem_init(&readCountLock, 0, 1); // Initialize semaphore for managing read count

    int **graph = (int **)malloc(V * sizeof(int *));
    int **dist = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        graph[i] = (int *)malloc(V * sizeof(int));
        dist[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
            dist[i][j] = graph[i][j];
        }
    }

    int maxWeight = 10; // Maximum weight for an edge
    for (int i = 1; i < E; i++) {
        int u = rand() % V;
        int v = rand() % V;
        while (u == v) {
            v = rand() % V;
        }
        int w = (rand() % maxWeight) + 1; // Weight between 1 and maxWeight
        graph[u][v] = dist[u][v] = w;
        graph[v][u] = dist[v][u] = w;
    }

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    floydWarshall(dist, V);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    printSolution(dist, V);

    for (int i = 0; i < V; i++) {
        free(graph[i]);
        free(dist[i]);
    }
    free(graph);
    free(dist);

    sem_destroy(&dbLock); // Destroy dbLock semaphore
    sem_destroy(&readCountLock); // Destroy readCountLock semaphore
    
    return 0;
}
