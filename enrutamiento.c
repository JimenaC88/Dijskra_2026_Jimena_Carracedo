#include <stdio.h>
#include <stdlib.h>
#include "enrutamiento.h"
#include "dijkstra.h"
#include <string.h>

int  construirTablaRutas(const Red *red, int origen, const ResultadoDijkstra *res, EntradaRuta *tabla){

    if (origen < 0 || origen >= red->numRouters) {
    return -1;
    }
    for (int i = 0; i < red->numRouters; i++) {
        tabla[i].destino = i;
        tabla[i].costo = res->distancia[i];

        if (i == origen) {
            tabla[i].costo = 0;
            tabla[i].destino = origen;
        } else if (res->distancia[i] == INF) {
            tabla[i].costo = INF;
            tabla[i].siguienteSalto = -1;
        } else {
            int actual = i;
            while (res->predecesor[actual] != origen && res->predecesor[actual] != -1) {
                actual = res->predecesor[actual];
            }
            tabla[i].siguienteSalto = actual;
        }
    }
    return 0;
}

void mostrarTablaRutas(const Red *red, const EntradaRuta *tabla){
    int origen = 0;
    for (int i = 0; i < red->numRouters; i++) {
        if (tabla[i].costo == 0) {
            origen = i;
            break;
        }
    }
        printf("\n=======================================================\n");
        printf("TABLA DE ENRUTAMIENTO OSPF PARA EL ROUTER LOCAL: %-8s\n", red->nombres[origen]);
        printf("=======================================================\n");
        printf("%-12s %-15s %-20s\n", "Destino", "Costo Total", "Ruta Optima (Router previo)");
        printf("-------------------------------------------------------\n");
        for(int i=0; i<red->numRouters; i++){
            printf("Router %-8s    ", red->nombres[tabla[i].destino]);
            if (tabla[i].costo == INF) {
            printf("%-15s %-20s\n", "Desconectado", "Ninguna");
        } else {
            printf("%-15d", tabla[i].costo);
            if(i==origen){

            printf(" %-30s \n", "Ninguno -Origen-");
            }else{
            printf("Desde: %s -> R%s \n", red->nombres[tabla[i].siguienteSalto], red->nombres[tabla[i].destino]);
            }
        }
    }
     printf("\n===================================================================\n");

    printf("RUTA FINAL OPTIMIZADA:\n");
    printf("\n===================================================================\n");
    for(int i=0; i<red->numRouters; i++){
        printf("Desde %s - > %s con costo %d\n", red->nombres[tabla[i].siguienteSalto], red->nombres[tabla[i].destino], tabla[i].costo);
        if(tabla[i].costo==INF){
        printf("\nNo hay ruta posible a %s\n", red->nombres[tabla[i].destino]);
        }
    }
    printf("\n===================================================================\n");
}
int  simularEnvioPaquete(const Red *red, int origen, int destino, const ResultadoDijkstra *res){
        EntradaRuta *tabla = malloc(red->numRouters * sizeof(EntradaRuta));
    if (tabla != NULL) {
        construirTablaRutas(red, origen, res, tabla);
        free(tabla);
    }


    printf("\n===========================================\n");

    if (res->distancia[destino] == INF) {
        printf("No hay ruta posible al paquete %s.", red->nombres[destino]);
        printf("\n===========================================\n");
        return -1;
    }

    if (origen == destino) {
        printf("Paquete local: %s ya es el destino.\n", red->nombres[origen]);
        printf("Costo del paquete: %d", res->distancia[destino]);
        printf("\n===========================================\n");
        return 0;
    }


    printf("Paquete enviado de %s a %s.\n", red->nombres[origen], red->nombres[destino]);
    printf("Costo del paquete: %d", res->distancia[destino]);
    printf("\n===========================================\n");

    return 0;

}
int  construirTablaRutas_paraRedManual(Red *red, int j, ResultadoDijkstra *res, EntradaRuta *tabla){
    if (j < 0 || j >= red->numRouters) {
    return -1;
    }
    for (int i = 0; i < red->numRouters; i++) {
        tabla[i].destino = i;
        tabla[i].costo = res->distancia[i];

        if (i == j) {
            tabla[i].costo = 0;
            tabla[i].destino = j;
            tabla[i].siguienteSalto = -1;
        } else if (res->distancia[i] == INF) {
            tabla[i].costo = INF;
            tabla[i].siguienteSalto = -1;
        } else {
            int actual = i;
            while (res->predecesor[actual] != j && res->predecesor[actual] != -1) {
                actual = res->predecesor[actual];
            }
            tabla[i].siguienteSalto = (res->predecesor[actual] == j) ? actual : -1;

        }
    }
    return 0;
}
static void imprimirCamino(const Red *red, const ResultadoDijkstra *res, int origen, int destino) {
    if (destino == origen) {
        printf("%s", red->nombres[origen]);
        return;
    }
    if (res->predecesor[destino] == -1) {
        printf("Sin ruta");
        return;
    }
    imprimirCamino(red, res, origen, res->predecesor[destino]);
    printf(" -> %s", red->nombres[destino]);
}
int  simularEnvioPaquete_paraRed_CargadaManualmente(Red *red){
        char routerorigen[MAX_NOMBRE], routerdestino[MAX_NOMBRE];
        int origen;
        int destino;
        ResultadoDijkstra res;
        memset(&res, 0, sizeof(ResultadoDijkstra));
        printf("Desde que router desea enviar el paquete?\n");
        if(fgets(routerorigen, MAX_NOMBRE, stdin)!=NULL){
            routerorigen[strcspn(routerorigen, "\n")]='\0';
        }
        printf("A que router desea enviar el paquete?\n");
        if(fgets(routerdestino, MAX_NOMBRE, stdin)!=NULL){
            routerdestino[strcspn(routerdestino, "\n")]='\0';
        }
        origen=buscar_Router(red, routerorigen);
        destino=buscar_Router(red, routerdestino);
        if(dijkstra_paraCargaManual(red, origen, &res)!=0){
            printf("Error en Dijkstra.");
            return 1;
        }else{
        EntradaRuta *tabla=(EntradaRuta*)malloc(MAX_ROUTERS*sizeof(EntradaRuta));
        construirTablaRutas_paraRedManual(red, origen, &res, tabla);
        printf("\n===========================================\n");
        if(res.distancia[destino]==INF){
            printf("No hay ruta posible al paquete %s.", red->nombres[destino]);
        }else{
            printf("Paquete enviado de %s a %s.\n", red->nombres[origen], red->nombres[destino]);
            printf("Costo del paquete: %d", tabla[destino].costo);
            printf("\n===========================================\n");
        }
        }
        printf("RUTA COMPLETA REQUERIDA:\n");
        printf("========================\n");
        imprimirCamino(red, &res, origen, destino);

    return 0;
}



void mostrarTablaRutas_deredManual(Red *red, EntradaRuta *tabla, ResultadoDijkstra *res, int origen) {
    printf("\n=======================================================\n");
    printf("TABLA DE ENRUTAMIENTO OSPF PARA EL ROUTER LOCAL: %-8s\n", red->nombres[origen]);
    printf("=======================================================\n");
    printf("%-12s %-15s %-20s\n", "Destino", "Costo Total", "Ruta Optima (Siguiente Salto)");
    printf("-------------------------------------------------------\n");

    for (int i = 0; i < red->numRouters; i++) {
        printf("Router %-8s ", red->nombres[tabla[i].destino]);

        if (tabla[i].costo == INF) {
            printf("%-15s %-20s\n", "Desconectado", "Ninguna");
        } else if (i == origen) {
            printf("%-15d %-20s\n", tabla[i].costo, "Ninguno (origen)");
        } else {
            printf("%-15d ", tabla[i].costo);
            imprimirCamino(red, res, origen, i);
            printf("\n");
        }
    }

    printf("=======================================================\n");
}
