#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "grafo.h"

int inicializarRed(Red *red, int cant_routers){

    red->numRouters=cant_routers;
    for(int i=0; i<cant_routers; i++){
    red->nombres[i]=(char*)malloc(MAX_NOMBRE*sizeof(char));
    if(red->nombres[i]==NULL){
        printf("Error al asignar memoria para el nombre del router.");
        return 1;
    }
    }
        for(int i=0; i<cant_routers; i++){

            printf("Ingrese el nombre del router %d:\n", i+1);
                if(fgets(red->nombres[i], MAX_NOMBRE, stdin)!=NULL){
                red->nombres[i][strcspn(red->nombres[i], "\n")]='\0';
                }
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
    return 0;
}
int  setNombreRouter(Red *red, int idx, const char *nombre){
     strcpy(red->nombres[idx], nombre);
     return 0;
}

int  agregarEnlace(Red *red, int i, int j, int costo){
     red->matriz[i][j]=costo;
     red->matriz[j][i]=red->matriz[i][j];
     return 0;
}
int  eliminarEnlace(Red *red, int i, int j){
     red->matriz[i][j]=INF;
     red->matriz[j][i]=red->matriz[j][i];
     return 0;
}
int  cargarTopologiaDesdeArchivo(Red *red, const char *ruta){
     printf("Hacer");
     return 0;
}
int  cargarTopologiaEjemplo(Red *red){
    int cant_routers;
    printf("Ingrese la cantidad de routers: \n");
    scanf("%d", &cant_routers);
    red->numRouters=cant_routers;
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    for(int i=0; i<cant_routers; i++){
        printf("Ingrese el nombre del router %d: \n", i+1);

            if(fgets(red->nombres[i], 32, stdin)!=NULL){
                red->nombres[i][strcspn(red->nombres[i], "\n")]='\0';
            }
    }
    int costo;
    bool conectadas;
    char check=' ';
    for(int i=0; i<cant_routers; i++){
        for(int j=0; j<i; j++){
            conectadas=false;
            printf("%s y %s estan conectadas? S/N\n", red->nombres[i], red->nombres[j]);
            scanf(" %c", &check);
            while(check!='S'&&check!='s'&&check!='N'&&check!='n'){
                printf("Ingrese S o N\n");
                scanf(" %c", &check);
            }
            if(check=='S'||check=='s'){
                conectadas=true;
            }
            if(conectadas){
            printf("Ingrese el costo desde el router %s al %s", red->nombres[i], red->nombres[j]);
            scanf("%d", &costo);
            red->matriz[i][j]=costo;
            red->matriz[j][i]=red->matriz[i][j];
            }else if(!conectadas){
                red->matriz[i][j]=INF;
            red->matriz[j][i]=red->matriz[i][j];
            }
        }
    }
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
void liberarRed(Red *red){

        for(int i=0; i<red->numRouters; i++){
        free(red->nombres[i]);
        }
        for(int i=0; i<red->numRouters; i++){
        for(int j=0; j<red->numRouters; j++){
            free(red->matriz[i][j]);
        }
        }
        free(red->matriz);
        free(red->numRouters);
        free(red);

}


