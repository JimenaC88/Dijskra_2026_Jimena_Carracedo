#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <stdbool.h>
#include "grafo.h"

typedef struct {
    int distancia[MAX_ROUTERS];
    bool predecesor[MAX_ROUTERS];
    int visitado[MAX_ROUTERS];
    char *nodo[MAX_ROUTERS];
} ResultadoDijkstra;

int dijkstra(Red *red, int origen, ResultadoDijkstra *res);

#endif /* DIJKSTRA_H */
