#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "dijkstra.h"
#include "enrutamiento.h"
#include "tests.h"
#include "carga.h"

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


static void mostrarMenu(void) {
    printf("\n===== SIMULADOR DE ENRUTAMIENTO - DIJKSTRA =====\n");
    printf("1. Cargar topologia desde archivo\n");
    printf("2. Cargar topologia de ejemplo\n");
    printf("3. Agregar enlace manualmente\n");
    printf("4. Mostrar topologia actual\n");
    printf("5. Calcular tabla de rutas desde un router\n");
    printf("6. Simular envio de paquete\n");
    printf("7. Ejecutar pruebas automaticas\n");
    printf("8. Carga manual de datos\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}
static void mostrarMenu1(void) {
    printf("\n===== SIMULADOR DE ENRUTAMIENTO - DIJKSTRA =====\n");
    printf("1. Agregar enlace manualmente\n");
    printf("2. Mostrar topologia actual\n");
    printf("3. Calcular tabla de rutas desde un router\n");
    printf("4. Simular envio de paquete\n");
    printf("5. Ejecutar pruebas automaticas\n");
    printf("6. Carga manual de datos\n");
    printf("0. Salir\n");
    printf("Opcion: ");
}

static void opcionCargarArchivo(Red *red) {
    char ruta[MAX_LINEA];
    printf("Ruta del archivo: ");
    if (scanf("%255s", ruta) != 1) {
            limpiarBuffer();
    return;
    }
    limpiarBuffer();

    if (cargarTopologiaDesdeArchivo(red, ruta) == 0)
        printf("Topologia cargada desde '%s'.\n", ruta);
    else
        printf("No se pudo cargar la topologia.\n");
}
int buscar_Router(Red *red, char nombre[]){
    int i=-1;
    for(int k=0; k<red->numRouters; k++){
            if(strcmp(red->nombres[k], nombre)==0){
                i=k;
                return i;
            }
        }
    while(i<0){
        printf("ERROR: el router no se halla en la red.\n");
        printf("Ingrese un router valido:\n");
            if(fgets(nombre, MAX_NOMBRE, stdin)!=NULL){
            nombre[strcspn(nombre, "\n")]='\0';
            }
        i=buscar_Router(red, nombre);
    }
    return i;
}

int agregarEnlace(Red *red, int i, int j, int costo) {
    if (red == NULL || red->numRouters <= 0){
            return -1;
    }
    if (i < 0 || i >= red->numRouters || j < 0 || j >= red->numRouters) return -1;

    if (i == j){
            return -1;
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
int eliminarEnlace(Red *red, int i, int j) {

    if (red == NULL || i < 0 || i >= red->numRouters || j < 0 || j >= red->numRouters) {
        return -1;
    }
    if (red->matriz[i][j] == INF){
            return -1;
    }
    red->matriz[i][j] = INF;
    red->matriz[j][i] = INF;
    return 0;
}


static void opcionTablaRutas(Red *red) {
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
            printf("Error en Dijkstra.\n");
            return;
    }

    EntradaRuta *tabla = (EntradaRuta *)malloc(red->numRouters * sizeof(EntradaRuta));
    construirTablaRutas(red, origen, &res, tabla);
    mostrarTablaRutas(red, tabla);
    free(tabla);
}

static void opcionSimularPaquete(Red *red) {
    if (red->numRouters == 0) {
            printf("Primero cargue una topologia.\n");
            return;
    }
    int o, d;
    char router1[MAX_NOMBRE], router2[MAX_NOMBRE];
    printf("Ingrese el router de inicio de red:\n");
    if(fgets(router1, MAX_NOMBRE, stdin)!=NULL){
        router1[strcspn(router1, "\n")]='\0';
    }
    printf("Ingrese el router de destino de red:\n");
    if(fgets(router2, MAX_NOMBRE, stdin)!=NULL){
        router2[strcspn(router2, "\n")]='\0';
    }
    o=buscar_Router(red, router1);
    d=buscar_Router(red, router2);
    if (o < 0 || o >= red->numRouters || d < 0 || d >= red->numRouters) {
        printf("Indices fuera de rango.\n");
        return;
    }

    ResultadoDijkstra res;
    dijkstra(red, o, &res);
    simularEnvioPaquete(red, o, d, &res);
}

int main(int argc, char *argv[]) {
     //Modo no interactivo: solo pruebas (útil para CI / make test) */
    if (argc > 1 && strcmp(argv[1], "--test") == 0)
        return ejecutarTodasLasPruebas();

    Red red;
    Red *redmanual=(Red*)malloc(sizeof(Red));
    if(redmanual==NULL){
        printf("Error al asignar memoria para garga manual\n");
    }

    int opcion;
    int opcion_manual;
    do {
        mostrarMenu();
        if(leerEntero("Ingrese la ocion elegida\n", &opcion)!=0||opcion<0||opcion>8){
            printf("Ingrese una opcion valida\n");
            continue;
        };
        switch (opcion) {
            case 1: opcionCargarArchivo(&red);
                    break;
            case 2:
                cargarTopologiaEjemplo(&red);
                printf("Topologia de ejemplo cargada (%d routers).\n", red.numRouters);
                break;
            case 3: agregarEnlace(&red, red.numRouters, red.numRouters, INF);
                    break;
            case 4: mostrarTopologia(&red);
                    break;
            case 5:
                opcionTablaRutas(&red);
                break;
            case 6:
                opcionSimularPaquete(&red);
            break;
                case 7:{
                int fallos = ejecutarTodasLasPruebas();
                printf("Pruebas fallidas: %d\n", fallos);
                break;
                }
            case 8:
                cargaManualdeDatos(redmanual);
                do{

                mostrarMenu1();

                while(leerEntero("Ingrese la opcion para la red cargada manual mente\n", &opcion_manual)!=0){
                    printf("Ingrese una opcion valida\n");
                    continue;
                }
                switch(opcion_manual){
                 case 1:
                    agregarEnlaceaRedCargadaManualmente(redmanual);
                    break;
                 case 2:
                    mostrarTopologia_deRedCargada(redmanual);
                    break;
                 case 3:
                    opcionTablaRutas_paraRedConCargaManual(redmanual);
                    break;
                 case 4:
                    simularEnvioPaquete_paraRed_CargadaManualmente(redmanual);
                    break;
                 case 5:
                    ejecutarTodasLasPruebas();
                    break;
                 case 6:
                     liberarRed(redmanual);
                     redmanual=(Red*)malloc(MAX_ROUTERS*sizeof(Red));
                     cargaManualdeDatos(redmanual);
                     break;
                 case 0:
                    liberarRed(redmanual);
                    printf("Saliendo de la red por carga manual\n");
                    break;
                    default: printf("Opcion invalida.\n");
                    break;
                }
                }while(opcion_manual!=0);
            case 0: printf("Saliendo...\n");
            break;
            default: printf("Opcion invalida.\n");
            break;
        }
    } while (opcion != 0);

    return 0;
}
