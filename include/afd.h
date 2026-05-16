#ifndef AFD_H
#define AFD_H

#include "common.h"

class AFD {
private:
    // Estados
    string* estados;
    int totalEstados;
    int capacidadEstados;
    
    // Alfabeto
    char* alfabeto;
    int totalSimbolos;
    int capacidadAlfabeto;
    
    // Configuración
    string estadoInicial;
    string* estadosFinales;
    int totalEstadosFinales;
    int capacidadEstadosFinales;
    
    // Tabla de transiciones
    int** tablaTransiciones;
    
    // Métodos privados
    void redimensionarEstados();
    void redimensionarAlfabeto();
    void redimensionarEstadosFinales();
    void recrearTablaTransiciones();
    int obtenerIndiceEstado(const string& nombre) const;
    int obtenerIndiceSimbol(char simbolo) const;
    bool esEstadoFinal(const string& nombre) const;
    
public:
    AFD();
    ~AFD();
    
    bool agregarEstado(const string& nombre);
    bool agregarEstadoFinal(const string& nombre);
    bool establecerEstadoInicial(const string& nombre);
    bool agregarSimboloAlfabeto(char simbolo);
    bool agregarTransicion(const string& desde, char simbolo, const string& hacia);
    bool verificarCadena(const string& cadena);
    bool estaVacio() const;
    
    int getTotalEstados() const { return totalEstados; }
    int getTotalSimbolos() const { return totalSimbolos; }
    const string* getEstados() const { return estados; }
    const char* getAlfabeto() const { return alfabeto; }
    const string* getEstadosFinales() const { return estadosFinales; }
    const string& getEstadoInicial() const { return estadoInicial; }
    int getTotalEstadosFinales() const { return totalEstadosFinales; }
    
    void mostrarConfiguracionAFD() const;
    void mostrarTablaTransiciones() const;
};

#endif
