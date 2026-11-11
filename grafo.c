#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grafo.h"
static void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static int leerEntero(const char *msg, int *out) {
    printf("%s", msg);
    if (scanf("%d", out) != 1) {
            limpiarBuffer();
    return -1;
    }
    limpiarBuffer();
    return 0;
}
int  setNombreRouter(Red *red, int idx, const char *nombre){
     strcpy(red->nombres[idx], nombre);
     red->nombres[idx][strcspn(red->nombres[idx], "\n")]='\0';
     return 0;
}
char  *leerNombreRouter(int idx, char *destino){
    printf("Ingrese el nombre del router %d:\n", idx + 1);
    fgets(destino, MAX_NOMBRE, stdin);
    return destino;
}

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

int  cargarTopologiaDesdeArchivo(Red *red, const char *ruta){
     printf("Hacer");
     return 0;
}
int  cargaManualdeDatos(Red *red){
    int cant_routers;
    while(leerEntero("Ingrese la cantidad de routers\n", &cant_routers)!=0 ||cant_routers<1 ||cant_routers>MAX_ROUTERS){
        printf("Ingrese un valor valido\n");
        continue;
    }
    red->numRouters=cant_routers;
    for(int i=0; i<cant_routers; i++){
        setNombreRouter(red, i, leerNombreRouter(i, red->nombres[i]));
    }
    for(int i=0; i<cant_routers; i++){
        for(int j=0; j<cant_routers; j++){
            if(i==j){
                red->matriz[i][j]=0;
            }else{
                red->matriz[i][j]=INF;
            }
        }
    }
    int costo;

    char check=' ';
    for(int i=0; i<cant_routers; i++){
        for(int j=0; j<i; j++){

            printf("%s y %s estan conectadas? S/N\n", red->nombres[i], red->nombres[j]);
            scanf(" %c", &check);
            while(check!='S'&&check!='s'&&check!='N'&&check!='n'){
                printf("Ingrese S o N\n");
                scanf(" %c", &check);
            }


            if(check=='S'||check=='s'){
            printf("Ingrese el costo desde el router %s al %s\n", red->nombres[i], red->nombres[j]);
            scanf("%d", &costo);
            red->matriz[i][j]=costo;
            red->matriz[j][i]=red->matriz[i][j];
            }
        }
    }

    return 0;
}
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
              printf("%s\t", "Desc");
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



