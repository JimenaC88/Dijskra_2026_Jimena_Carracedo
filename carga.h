#ifndef CARGA_H
#define CARGA_H

#include "grafo.h"
int  cargaManualdeDatos(Red *red);
int  setNombreRouter(Red *red, int idx, const char *nombre);
char  *leerNombreRouter(int idx, char *destino);
int  cargaManualdeDatos(Red *red);
int agregarEnlaceaRedCargadaManualmente(Red *red);
void opcionTablaRutas_paraRedConCargaManual(Red *red);
#endif
