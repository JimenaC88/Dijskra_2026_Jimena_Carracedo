#ifndef CARGA_H
#define CARGA_H

#include "grafo.h"
int  cargaManualdeDatos(Red *red);
int  setNombreRouter_Cargado(Red *red, int idx, char *nombre);
char  *leerNombreRouter(int idx, char *destino);
int agregarEnlaceaRedCargadaManualmente(Red *red);
void opcionTablaRutas_paraRedConCargaManual(Red *red);
#endif
