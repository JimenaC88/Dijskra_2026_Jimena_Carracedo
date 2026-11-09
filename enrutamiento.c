#include <stdio.h>
#include <stdlib.h>
#include "enrutamiento.h"
#include "dijkstra.h"
#include <string.h>
int  construirTablaRutas(Red *red, int j, ResultadoDijkstra *res, EntradaRuta *tabla){
    for(int i=0; i<red->numRouters; i++){
        strcpy(tabla[i].destino, red->nombres[i]);
        tabla[i].costo=res->distancia[i];
        if(i==j){
            tabla[i].costo=0;
            strcpy(tabla[i].siguienteSalto, "Ninguno");
        }else if(res->distancia[i]==INF){
            tabla[i].costo=INF;
            strcpy(tabla[i].siguienteSalto, "Desconectado");
        }else{
           int actual=i;
           int pred=res->predecesor[i];

        while(pred!=-1&&pred!=j){
            actual=pred;
            pred=res->predecesor[actual];
        }
        strcpy(tabla[i].siguienteSalto, red->nombres[actual]);

    }
    }

    return 0;
}

void mostrarTablaRutas(Red *red, EntradaRuta *tabla, int origen){
     printf("\n=======================================================\n");
        printf("TABLA DE ENRUTAMIENTO OSPF PARA EL ROUTER LOCAL: %-8s\n", red->nombres[origen]);
        printf("=======================================================\n");
        printf("%-12s %-15s %-20s\n", "Destino", "Costo Total", "Ruta Optima (Siguiente Salto)");
        printf("-------------------------------------------------------\n");
        for(int i=0; i<red->numRouters; i++){
            printf("Router %-8s    ", tabla[i].destino);
            if (tabla[i].costo == INF) {
            printf("%-15s %-20s\n", "Desconectado", "Ninguna");
        } else {
            printf("%-15d %-25s\n", tabla[i].costo, tabla[i].siguienteSalto);
            printf("\n");
        }
    }
    printf("=======================================================\n");
}
