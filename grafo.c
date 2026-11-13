#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

int inicializarRed(Red *red, int cant_routers){
    red->numRouters=cant_routers;
    char bufferNombre[MAX_NOMBRE];
    for (int i = 0; i < cant_routers; i++) {
        snprintf(bufferNombre, MAX_NOMBRE, "R%d", i + 1);
        setNombreRouter(red, i, bufferNombre);
    }
    for (int i = 0; i < MAX_ROUTERS; i++) {
        for (int j = 0; j < MAX_ROUTERS; j++) {
            if (i == j) {
                red->matriz[i][j] = 0;
            } else {
                red->matriz[i][j] = INF;
            }
        }
    }
    return 0;
}

/*int  cargarTopologiaDesdeArchivo(Red *red, const char *ruta){
     printf("Hacer");
     return 0;
}*/

int cargarTopologiaEjemplo(Red *red) {

    inicializarRed(red, 5);


    agregarEnlace(red, 0, 1, 2);
    agregarEnlace(red, 0, 2, 5);
    agregarEnlace(red, 1, 2, 1);
    agregarEnlace(red, 1, 3, 4);
    agregarEnlace(red, 2, 3, 1);
    agregarEnlace(red, 3, 4, 3);

    return 0;
}

void mostrarTopologia(const Red *red){
        printf("\n=======================================================\n");
        printf("TABLA DE ENRUTAMIENTO OSPF\n");
        printf("=======================================================\n");
        printf("%-8s %15s\n", " ", "Costo Total a:");
        printf("-------------------------------------------------------\n");
        for(int i=0; i<red->numRouters; i++){
            printf("%s\t %s", " ", red->nombres[i]);
        }
        printf("\n");
        for(int i=0; i<red->numRouters; i++){

            printf("%s\t", red->nombres[i]);

            for(int j=0; j<red->numRouters; j++){

              if(red->matriz[i][j]!=INF){
              printf("%d\t", red->matriz[i][j]);
              }else{
              printf("%s\t", "INF");
              }
            }
            printf("\n");
        }
    printf("=======================================================\n");
}
void mostrarTopologia_deRedCargada(Red *red){
        printf("\n=======================================================\n");
        printf("TABLA DE ENRUTAMIENTO OSPF\n");
        printf("=======================================================\n");
        printf("%-8s %15s\n", " ", "Costo Total a:");
        printf("-------------------------------------------------------\n");
        for(int i=0; i<red->numRouters; i++){
            printf("%s\t %s", " ", red->nombres[i]);
        }
        printf("\n");
        for(int i=0; i<red->numRouters; i++){

            printf("%s\t", red->nombres[i]);

            for(int j=0; j<red->numRouters; j++){

              if(red->matriz[i][j]!=INF){
              printf("%d\t", red->matriz[i][j]);
              }else{
              printf("%s\t", "Desc");
              }
            }
            printf("\n");
        }
    printf("=======================================================\n");

}
void liberarRed(Red *red){
    free(red);
}



