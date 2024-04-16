#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define INF INT_MAX

void floydWarshall(int **graph, int V);
void printSolution(int **dist, int V);

int main() {
    int V;
    printf("Enter the number of vertices: ");
    scanf("%d", &V);

    srand(time(NULL)); // Seed for random number generation

    // Assuming a moderately dense graph, for demonstration
    int E = V * (V - 1) / 4; // Adjust density as needed

    int **graph = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        graph[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            if (i == j) graph[i][j] = 0;
            else graph[i][j] = INF;
        }
    }

    int maxWeight = 10; // Max weight for an edge
    for (int i = 0; i < E; i++) {
        int u = rand() % V;
        int v = rand() % V;
        while (u == v) { // Ensure u and v are distinct
            v = rand() % V;
        }
        int w = (rand() % maxWeight) + 1; // Weight between 1 and maxWeight
        // Since the graph is undirected
        graph[u][v] = w;
        graph[v][u] = w;
    }

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    floydWarshall(graph, V);
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken: %f seconds\n", cpu_time_used);

    // Cleanup
    for (int i = 0; i < V; i++) {
        free(graph[i]);
    }
    free(graph);

    return 0;
}

void floydWarshall(int **graph, int V) {
    int **dist = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        dist[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            dist[i][j] = graph[i][j];
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                }
            }
        }
    }

    printSolution(dist, V);

    for (int i = 0; i < V; i++) {
        free(dist[i]);
    }
    free(dist);
}

void printSolution(int **dist, int V) {
    printf("Output:\n");
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
