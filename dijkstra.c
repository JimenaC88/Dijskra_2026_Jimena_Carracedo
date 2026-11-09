#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "dijkstra.h"

int minima_distancia(Red *red, ResultadoDijkstra *res){
    int minimo=INF, indice=-1;
    for(int i=0; i<red->numRouters; i++){
        if(res->visitado[i]<0&&res->distancia[i]<minimo){
            minimo=res->distancia[i];
            indice=i;

        }
    }
    return indice;
}

int dijkstra(Red *red, int indice, ResultadoDijkstra *res){
    for(int i=0; i<red->numRouters; i++){
        res->visitado[i]=-1;
        res->distancia[i]=INF;
        res->predecesor[i]=-1;
    }
    printf("El indice del router elegido es: %d", indice);
    res->distancia[indice]=0;
    int v;
    for(int i=0; i<red->numRouters; i++){
        v=minima_distancia(red, res);
        if(v==-1){
            break;
        }
    res->visitado[v]=0;
    for(int k=0; k<red->numRouters; k++){
            if(res->visitado[k]!=0&&red->matriz[v][k]!=INF&&res->distancia[v]!=INF&&res->distancia[v]+red->matriz[v][k]<res->distancia[k]){
                res->distancia[k]=res->distancia[v]+red->matriz[v][k];
                res->predecesor[k]=v;
            }
        }
    }

    return 0;
}
