#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "dijkstra.h"
#include "enrutamiento.h"
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
     if(idx<0||idx>=red->numRouters){
        printf("Indice fuera de rango.");
        return 1;
     }
     if(strlen(nombre)>=MAX_NOMBRE){
        return -1;
     }
     strcpy(red->nombres[idx], nombre);
     red->nombres[idx][strcspn(red->nombres[idx], "\n")]='\0';
     return 0;
}
char  *leerNombreRouter(int idx, char *destino){
    printf("Ingrese el nombre del router %d:\n", idx + 1);
    fgets(destino, MAX_NOMBRE, stdin);
    return destino;
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
int agregarEnlaceaRedCargadaManualmente(Red *red){
    if (red == NULL || red->numRouters <= 0){
            return -1;
    }
    printf("Ingrese el nombre del primer router al cual desea cambiarle el costo:\n");
    char nombre[MAX_NOMBRE];

    if(fgets(nombre, MAX_NOMBRE, stdin)!=NULL){
            nombre[strcspn(nombre, "\n")]='\0';
    }
    int i=buscar_Router(red, nombre);
    printf("Ingrese el nombre del router hacia el cual desea modificarle el costo: \n");
    if(fgets(nombre, MAX_NOMBRE, stdin)!=NULL){
            nombre[strcspn(nombre, "\n")]='\0';
    }
    int j=buscar_Router(red, nombre);

    if (i < 0 || i >= red->numRouters || j < 0 || j >= red->numRouters) return -1;

    if (i == j){
            return -1;
    }
    int costo;
    while(leerEntero("Ingrese el costo de conexion.\n", &costo)!=0){
         printf("Ingrese dato valido.\n");
         continue;
    }
    if(costo <0){
        return -1;
    }
    if (costo == 0){
            return -1;
    }
    if (red->matriz[i][j] != INF && red->matriz[i][j] != 0){
            return -1;
    }
    red->matriz[i][j] = costo;
    red->matriz[j][i] = red->matriz[i][j];
    return 0;
}
void opcionTablaRutas_paraRedConCargaManual(Red *red) {
    if (red->numRouters == 0) {
            printf("Primero cargue una topologia.\n");
            return;
    }
    ResultadoDijkstra res;
    memset(&res, 0, sizeof(ResultadoDijkstra));
    char router_inicial[MAX_NOMBRE];
    printf("Ingrese el nombre del router desde el cual quiere partir:\n");
    if(fgets(router_inicial, MAX_NOMBRE, stdin)!=NULL){
        router_inicial[strcspn(router_inicial, "\n")]='\0';
    }
    int origen=buscar_Router(red, router_inicial);
    if (dijkstra(red, origen, &res) != 0) {
            printf("Error en Dijkstra para Carga manual.\n");
            return;
    }



    EntradaRuta *tabla=(EntradaRuta*)malloc(MAX_ROUTERS*sizeof(EntradaRuta));
    construirTablaRutas(red, origen, &res, tabla);
    mostrarTablaRutas_deredManual(red, tabla, &res, origen);
    free(tabla);
}
