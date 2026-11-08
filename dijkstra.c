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
            res->visitado[i]=0;
        }
    }
    return indice;
}
void imprimir_ruta(ResultadoDijkstra *res, int j, Red *red) {

    if (res->predecesor[j] == -1) {
        printf("%s", red->nombres[j]);
        return;
    }
    imprimir_ruta(res, res->predecesor[j], red);
    printf(" -> %s", red->nombres[j]);
}
void mostrar_resultado(Red *red, ResultadoDijkstra *res, char nombre_router_principal[]){
        printf("\n=======================================================\n");
        printf("TABLA DE ENRUTAMIENTO OSPF PARA EL ROUTER LOCAL: %-8s\n", nombre_router_principal);
        printf("=======================================================\n");
        printf("%-12s %-15s %-20s\n", "Destino", "Costo Total", "Ruta Optima (Siguiente Salto)");
        printf("-------------------------------------------------------\n");
        for(int i=0; i<red->numRouters; i++){
            printf("Router %-8s    ", red->nombres[i]);
            if (res->distancia[i] == INF) {
            printf("%-15s %-20s\n", "Desconectado", "Ninguna");
        } else {
            printf("%-15d ", res->distancia[i]);
            imprimir_ruta(res, i, red);
            printf("\n");
        }
    }
    printf("=======================================================\n");
}
int dijkstra(Red *red, ResultadoDijkstra *res){
    res->visitado[MAX_ROUTERS] = (int*)malloc(red->numRouters * sizeof(int));
    res->distancia[MAX_ROUTERS] = (int*)malloc(red->numRouters * sizeof(int));
    res->predecesor[MAX_ROUTERS] = (int*)malloc(red->numRouters * sizeof(int));
    for(int i=0; i<red->numRouters; i++){
        res->visitado[i]=-1;
        res->distancia[i]=INF;
        res->predecesor[i]=-1;
    }
    char router_principal[MAX_NOMBRE];
    printf("Ingrese el nombre del router inicial:\n");

    if(fgets(router_principal, MAX_NOMBRE, stdin)!=NULL){
        router_principal[strcspn(router_principal, "\n")]='\0';

    }

    int indice=buscar_Router(red, router_principal);
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
    mostrar_resultado(red, res, router_principal);
    return 0;
}
