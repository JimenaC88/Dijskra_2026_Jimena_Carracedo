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
        EntradaRuta *tabla = malloc(MAX_ROUTERS * sizeof(EntradaRuta));
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
    imprimirCamino(red, res, origen, destino);

    return 0;

}
int simularEnvioPaquete_paraRed_CargadaManualmente(Red *red) {
    if (red == NULL || red->numRouters <= 0) {
        printf("Primero cargue una topologia.\n");
        return 1;
    }

    char routerorigen[MAX_NOMBRE], routerdestino[MAX_NOMBRE];
    ResultadoDijkstra res;
    memset(&res, 0, sizeof(ResultadoDijkstra));

    printf("Desde que router desea enviar el paquete?\n");
    if (fgets(routerorigen, MAX_NOMBRE, stdin) != NULL) {
        routerorigen[strcspn(routerorigen, "\n")] = '\0';
    }
    printf("A que router desea enviar el paquete?\n");
    if (fgets(routerdestino, MAX_NOMBRE, stdin) != NULL) {
        routerdestino[strcspn(routerdestino, "\n")] = '\0';
    }

    int origen = buscar_Router(red, routerorigen);
    int destino = buscar_Router(red, routerdestino);

    if (origen < 0 || origen >= red->numRouters || destino < 0 || destino >= red->numRouters) {
        printf("Uno o ambos routers no existen en la red.\n");
        return 1;
    }

    if (dijkstra(red, origen, &res) != 0) {
        printf("Error en Dijkstra.\n");
        return 1;
    }

    if (res.distancia[destino] == INF) {
        printf("\n===========================================\n");
        printf("No hay ruta posible hacia %s.\n", red->nombres[destino]);
        printf("===========================================\n");
        return 0;
    }

    EntradaRuta *tabla = malloc(MAX_ROUTERS * sizeof(EntradaRuta));
    if (tabla == NULL) {
        printf("Error al asignar memoria para la tabla.\n");
        return 1;
    }
    construirTablaRutas(red, origen, &res, tabla);

    printf("\n===========================================\n");
    printf("Paquete enviado de %s a %s.\n", red->nombres[origen], red->nombres[destino]);
    printf("Costo del paquete: %d\n", tabla[destino].costo);
    printf("===========================================\n");
    printf("RUTA COMPLETA REQUERIDA:\n");
    printf("========================\n");
    imprimirCamino(red, &res, origen, destino);

    free(tabla);
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
