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

            strcpy(tabla[i].siguienteSalto, res->nodo[i]);
            }

        }

    return 0;
}

void mostrarTablaRutas(Red *red, EntradaRuta *tabla, int origen){
     printf("\n=======================================================\n");
        printf("TABLA DE ENRUTAMIENTO OSPF PARA EL ROUTER LOCAL: %-8s\n", red->nombres[origen]);
        printf("=======================================================\n");
        printf("%-12s %-15s %-20s\n", "Destino", "Costo Total", "Ruta Optima (Router previo)");
        printf("-------------------------------------------------------\n");
        for(int i=0; i<red->numRouters; i++){
            printf("Router %-8s    ", tabla[i].destino);
            if (tabla[i].costo == INF) {
            printf("%-15s %-20s\n", "Desconectado", "Ninguna");
        } else {
            printf("%-15d", tabla[i].costo);
            if(i==origen){

            printf(" %-30s \n", "Ninguno -Origen-");
            }else{
            printf("Desde: %s -> %s \n", tabla[i].siguienteSalto, tabla[i].destino);
            }
        }
    }
     printf("\n===================================================================\n");

    printf("RUTA FINAL OPTIMIZADA:\n");
    printf("\n===================================================================\n");
    for(int i=0; i<red->numRouters; i++){
        printf("Desde %s - > %s con costo %d\n", tabla[i].siguienteSalto, tabla[i].destino, tabla[i].costo);
        if(tabla[i].costo==INF){
        printf("\nNo hay ruta posible a %s\n", tabla[i].destino);
        }
    }
    printf("\n===================================================================\n");
}

int  simularEnvioPaquete(Red *red, int origen, int destino, ResultadoDijkstra *res){
        if(dijkstra(red, origen, res)!=0){
            printf("Error en Dijkstra.");
            return 1;
        }else{
        EntradaRuta tabla[red->numRouters];
        construirTablaRutas(red, origen, res, &tabla);
        printf("\n===========================================\n");
        if(res->distancia[destino]==INF){
            printf("No hay ruta posible al paquete %s.", res->nodo[destino]);
        }else{
            printf("Paquete enviado de %s a %s.\n", red->nombres[origen], red->nombres[destino]);
            printf("Costo del paquete: %d", res->distancia[destino]);
            printf("\n===========================================\n");
        }
        }
    return 0;
}
