#ifndef VALIDACION_H
#define VALIDACION_H

#include "afd.h"

int leerOpcionMenu(int min, int max);
int leerEntero(int min);
bool leerBooleano(const string& contexto);
string leerStringNoVacio(const string& campo);
string leerCadenaVerificacion();
void limpiarBuffer();
int obtenerIndiceEstado(AFD* afd, const string& nombre);
string leerNombreEstado(AFD* afd, const string& contexto);
void mostrarEstados(AFD* afd);

#endif
