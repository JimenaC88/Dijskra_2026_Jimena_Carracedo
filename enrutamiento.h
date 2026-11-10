#ifndef ENRUTAMIENTO_H
#define ENRUTAMIENTO_H

#include "dijkstra.h"

typedef struct {
    char destino[MAX_NOMBRE];
    int costo;
    char siguienteSalto[MAX_NOMBRE];   /* -1 si inalcanzable */
} EntradaRuta;

int  construirTablaRutas(Red *red, int origen, ResultadoDijkstra *res, EntradaRuta *tabla);
void mostrarTablaRutas(Red *red, EntradaRuta *tabla, int origen);
int  simularEnvioPaquete( Red *red, int origen, int destino, ResultadoDijkstra *res);

#endif /* ENRUTAMIENTO_H */
