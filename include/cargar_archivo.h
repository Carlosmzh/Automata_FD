#ifndef CARGAR_ARCHIVO_H
#define CARGAR_ARCHIVO_H

#include "afd.h"

bool cargarAFDDesdeArchivo(AFD& afd, const string& nombreArchivo);
void cargarAFDHardcodeado(AFD& afd);
vector<string> split(const string& cadena, char delimitador);

#endif
