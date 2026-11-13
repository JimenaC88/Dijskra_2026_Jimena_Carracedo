#ifndef ENRUTAMIENTO_H
#define ENRUTAMIENTO_H

#include "grafo.h"
#include "dijkstra.h"
typedef struct {
    int destino;
    int costo;
    int siguienteSalto;   /* -1 si inalcanzable */
} EntradaRuta;

int  construirTablaRutas(const Red *red, int origen, const ResultadoDijkstra *res, EntradaRuta *tabla);
void mostrarTablaRutas(const Red *red, const EntradaRuta *tabla);
int  simularEnvioPaquete(const Red *red, int origen, int destino, const ResultadoDijkstra *res);
int  simularEnvioPaquete_paraRed_CargadaManualmente(Red *red);
void mostrarTablaRutas_deredManual(Red *red, EntradaRuta *tabla, ResultadoDijkstra *res, int origen);
int buscar_Router(Red *red, char nombre[]);
#endif /* ENRUTAMIENTO_H */
