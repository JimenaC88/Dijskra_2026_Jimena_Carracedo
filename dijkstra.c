#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grafo.h"
#include "dijkstra.h"

int minima_distancia(Red *red, ResultadoDijkstra *res){
    int minimo=INF, indice=-1;
    for(int i=0; i<red->numRouters; i++){
        if(!res->visitado[i]&&res->distancia[i]<minimo){
            minimo=res->distancia[i];
            indice=i;

        }
    }
    return indice;
}

int dijkstra(Red *red, int indice, ResultadoDijkstra *res){
    for(int i=0; i<red->numRouters; i++){
        res->visitado[i]=false;
        res->distancia[i]=INF;
        res->predecesor[i]=-1;
        if (res->nodo[i] == NULL) {
            res->nodo[i] = (char*)malloc(MAX_NOMBRE * sizeof(char));
        }
    }

    printf("El indice del router elegido es: %d", indice);
    res->distancia[indice]=0;
    int v;
    for(int i=0; i<red->numRouters; i++){
        v=minima_distancia(red, res);
        if(v==-1){
            break;
        }
    res->visitado[v]=true;
    for(int k=0; k<red->numRouters; k++){
            if(!res->visitado[k]&&red->matriz[v][k]!=INF&&res->distancia[v]!=INF&&res->distancia[v]+red->matriz[v][k]<res->distancia[k]){
                res->distancia[k]=res->distancia[v]+red->matriz[v][k];
                res->predecesor[k]=v;
                strcpy(res->nodo[k], red->nombres[v]);
            }
        }
    }
    printf("Sale de dijskra\n");
    return 0;
}
