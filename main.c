#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"
#include "dijkstra.h"
#include "enrutamiento.h"
#include "tests.h"

static void limpiarBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF){

    }
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
    printf("0. Salir\n");
    printf("Opcion: ");
}

static void opcionCargarArchivo(Red *red) {
    char ruta[MAX_LINEA];
    printf("Ruta del archivo: ");
    if (scanf("%255s", ruta) != 1) { limpiarBuffer(); return; }
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
                break;
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
static void opcionAgregarEnlace(Red *red) {
    if (red->numRouters == 0) {
            printf("Primero cargue una topologia.\n");
            return;
    }
    int i=-1, j=-1, c;
    printf("Ingrese el nombre del primer router al cual desea cambiarle el costo:\n");
    char nombre[MAX_NOMBRE];

    if(fgets(nombre, MAX_NOMBRE, stdin)!=NULL){
            nombre[strcspn(nombre, "\n")]='\0';
    }
    i=buscar_Router(red, nombre);
    printf("Ingrese el nombre del router hacia el cual desea modificarle el costo: \n");
    if(fgets(nombre, MAX_NOMBRE, stdin)!=NULL){
            nombre[strcspn(nombre, "\n")]='\0';
    }
    j=buscar_Router(red, nombre);
    if (leerEntero("Costo: ", &c) != 0) return;

    if (agregarEnlace(red, i, j, c) == 0){
        printf("Enlace %s-%s agregado (costo %d).\n", red->nombres[i], red->nombres[j], c);
    }else{
        printf("Error: indices invalidos, costo <= 0 o enlace ya existente.\n");
    }
}

static void opcionTablaRutas(Red *red) {
    if (red->numRouters == 0) {
            printf("Primero cargue una topologia.\n");
            return;
    }
    ResultadoDijkstra res;
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
    mostrarTablaRutas(red, tabla, origen);
}

/*static void opcionSimularPaquete(const Red *red) {
    if (red->numRouters == 0) {
            printf("Primero cargue una topologia.\n");
            return;
    }
    int o, d, indice1, indice2;
    char router1[MAX_NOMBRE], router2[MAX_NOMBRE];
    printf("Ingrese el router de inicio de red:\n");
    if(fgets(router1, MAX_NOMBRE, stdin)!=NULL){
        router1[strcspn(router1, "\n"]='\0';
    }
    printf("Ingrese el router de destino de red:\n");
    if(fgets(router2, MAX_NOMBRE, stdin)!=NULL){
        router2[strcspn(router2, "\n")]='\0';
    }
    //o=buscar_Router(red, router1);
    d=buscar_Router(red, router2);
    //if (leerEntero("Router origen: ", &o) != 0) return;
    if (leerEntero("Router destino: ", &d) != 0) return;
    if (o < 0 || o >= red->numRouters || d < 0 || d >= red->numRouters) {
        printf("Indices fuera de rango.\n");
        return;
    }

    ResultadoDijkstra res;
    dijkstra(red, &res, router2);
    //simularEnvioPaquete(red, o, d, &res);
}*/

int main(/*int argc, char *argv[]*/) {
    /* Modo no interactivo: solo pruebas (útil para CI / make test) */
    //if (argc > 1 && strcmp(argv[1], "--test") == 0)
        //return ejecutarTodasLasPruebas();

    Red red;
    inicializarRed(&red, 3);
    int opcion;
    do {
        mostrarMenu();
        if (scanf("%d", &opcion) != 1) {
                limpiarBuffer();
                continue; }
        limpiarBuffer();

        switch (opcion) {
            case 1: opcionCargarArchivo(&red); break;
            case 2:
                cargarTopologiaEjemplo(&red);
                printf("Topologia de ejemplo cargada (%d routers).\n", red.numRouters);
                break;
            case 3: opcionAgregarEnlace(&red);
            break;
            case 4: mostrarTopologia(&red);
            break;
            case 5: opcionTablaRutas(&red);
            break;
            //case 6: opcionSimularPaquete(&red); break;
            /*case 7: {
                int fallos = ejecutarTodasLasPruebas();
                printf("Pruebas fallidas: %d\n", fallos);
                break;
            }*/
            case 0: printf("Saliendo...\n"); break;
            default: printf("Opcion invalida.\n"); break;
        }
    } while (opcion != 0);

    liberarRed(&red);
    return 0;
}
