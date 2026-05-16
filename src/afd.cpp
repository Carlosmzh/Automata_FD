#include "../include/afd.h"

// Constructor
AFD::AFD() 
    : estados(nullptr), totalEstados(0), capacidadEstados(CAPACIDAD_INICIAL),
      alfabeto(nullptr), totalSimbolos(0), capacidadAlfabeto(CAPACIDAD_ALFABETO),
      estadoInicial(""), estadosFinales(nullptr), totalEstadosFinales(0),
      capacidadEstadosFinales(CAPACIDAD_INICIAL),
      tablaTransiciones(nullptr) {
    
    estados = new string[capacidadEstados];
    alfabeto = new char[capacidadAlfabeto];
    estadosFinales = new string[capacidadEstadosFinales];
}

// Destructor
AFD::~AFD() {
    if (tablaTransiciones != nullptr) {
        for (int i = 0; i < capacidadEstados; i++) {
            delete[] tablaTransiciones[i];
        }
        delete[] tablaTransiciones;
    }
    
    delete[] estados;
    delete[] alfabeto;
    delete[] estadosFinales;
}

// Redimensionamiento de estados
void AFD::redimensionarEstados() {
    capacidadEstados *= 2;
    string* nuevoArreglo = new string[capacidadEstados];
    
    for (int i = 0; i < totalEstados; i++) {
        nuevoArreglo[i] = estados[i];
    }
    
    delete[] estados;
    estados = nuevoArreglo;
    recrearTablaTransiciones();
}

// Redimensionamiento de alfabeto
void AFD::redimensionarAlfabeto() {
    capacidadAlfabeto *= 2;
    char* nuevoArreglo = new char[capacidadAlfabeto];
    
    for (int i = 0; i < totalSimbolos; i++) {
        nuevoArreglo[i] = alfabeto[i];
    }
    
    delete[] alfabeto;
    alfabeto = nuevoArreglo;
    recrearTablaTransiciones();
}

// Redimensionamiento de estados finales
void AFD::redimensionarEstadosFinales() {
    capacidadEstadosFinales *= 2;
    string* nuevoArreglo = new string[capacidadEstadosFinales];
    
    for (int i = 0; i < totalEstadosFinales; i++) {
        nuevoArreglo[i] = estadosFinales[i];
    }
    
    delete[] estadosFinales;
    estadosFinales = nuevoArreglo;
}

// Recrea tabla de transiciones
void AFD::recrearTablaTransiciones() {
    if (tablaTransiciones != nullptr) {
        for (int i = 0; i < capacidadEstados; i++) {
            delete[] tablaTransiciones[i];
        }
        delete[] tablaTransiciones;
    }
    
    if (totalEstados > 0 && totalSimbolos > 0) {
        tablaTransiciones = new int*[capacidadEstados];
        for (int i = 0; i < capacidadEstados; i++) {
            tablaTransiciones[i] = new int[capacidadAlfabeto];
            for (int j = 0; j < capacidadAlfabeto; j++) {
                tablaTransiciones[i][j] = -1;
            }
        }
    }
}

// Obtener índice de estado
int AFD::obtenerIndiceEstado(const string& nombre) const {
    for (int i = 0; i < totalEstados; i++) {
        if (estados[i] == nombre) return i;
    }
    return -1;
}

// Obtener índice de símbolo
int AFD::obtenerIndiceSimbol(char simbolo) const {
    for (int i = 0; i < totalSimbolos; i++) {
        if (alfabeto[i] == simbolo) return i;
    }
    return -1;
}

// Verificar si es estado final
bool AFD::esEstadoFinal(const string& nombre) const {
    for (int i = 0; i < totalEstadosFinales; i++) {
        if (estadosFinales[i] == nombre) return true;
    }
    return false;
}

// Agregar estado
bool AFD::agregarEstado(const string& nombre) {
    if (obtenerIndiceEstado(nombre) != -1) {
        cerr << "Error: Estado '" << nombre << "' ya existe.\n";
        return false;
    }
    
    if (totalEstados >= capacidadEstados) {
        redimensionarEstados();
    }
    
    estados[totalEstados] = nombre;
    totalEstados++;
    
    if (totalEstados == 1 && totalSimbolos > 0) {
        recrearTablaTransiciones();
    }
    
    return true;
}

// Agregar estado final
bool AFD::agregarEstadoFinal(const string& nombre) {
    if (obtenerIndiceEstado(nombre) == -1) {
        cerr << "Error: Estado '" << nombre << "' no existe.\n";
        return false;
    }
    
    if (esEstadoFinal(nombre)) {
        cerr << "Error: Estado '" << nombre << "' ya es final.\n";
        return false;
    }
    
    if (totalEstadosFinales >= capacidadEstadosFinales) {
        redimensionarEstadosFinales();
    }
    
    estadosFinales[totalEstadosFinales] = nombre;
    totalEstadosFinales++;
    
    return true;
}

// Establecer estado inicial
bool AFD::establecerEstadoInicial(const string& nombre) {
    if (obtenerIndiceEstado(nombre) == -1) {
        cerr << "Error: Estado '" << nombre << "' no existe.\n";
        return false;
    }
    
    estadoInicial = nombre;
    return true;
}

// Agregar símbolo al alfabeto
bool AFD::agregarSimboloAlfabeto(char simbolo) {
    if (obtenerIndiceSimbol(simbolo) != -1) {
        cerr << "Error: Símbolo '" << simbolo << "' ya existe.\n";
        return false;
    }
    
    if (totalSimbolos >= capacidadAlfabeto) {
        redimensionarAlfabeto();
    }
    
    alfabeto[totalSimbolos] = simbolo;
    totalSimbolos++;
    
    if (totalSimbolos == 1 && totalEstados > 0) {
        recrearTablaTransiciones();
    }
    
    return true;
}

// Agregar transición
bool AFD::agregarTransicion(const string& desde, char simbolo, const string& hacia) {
    int idxDesde = obtenerIndiceEstado(desde);
    if (idxDesde == -1) {
        cerr << "Error: Estado origen '" << desde << "' no existe.\n";
        return false;
    }
    
    int idxHacia = obtenerIndiceEstado(hacia);
    if (idxHacia == -1) {
        cerr << "Error: Estado destino '" << hacia << "' no existe.\n";
        return false;
    }
    
    int idxSimbolo = obtenerIndiceSimbol(simbolo);
    if (idxSimbolo == -1) {
        cerr << "Error: Símbolo '" << simbolo << "' no en alfabeto.\n";
        return false;
    }
    
    if (tablaTransiciones == nullptr) {
        cerr << "Error: Tabla no inicializada.\n";
        return false;
    }
    
    tablaTransiciones[idxDesde][idxSimbolo] = idxHacia;
    return true;
}

// Verificar cadena
bool AFD::verificarCadena(const string& cadena) {
    if (estadoInicial.empty()) {
        cerr << "Error: Estado inicial no establecido.\n";
        return false;
    }
    
    string estadoActual = estadoInicial;
    
    for (char simbolo : cadena) {
        int idxSimbolo = obtenerIndiceSimbol(simbolo);
        if (idxSimbolo == -1) {
            cerr << "Error: Símbolo '" << simbolo << "' no válido.\n";
            return false;
        }
        
        int idxEstado = obtenerIndiceEstado(estadoActual);
        int idxProximo = tablaTransiciones[idxEstado][idxSimbolo];
        
        if (idxProximo == -1) {
            return false;
        }
        
        estadoActual = estados[idxProximo];
    }
    
    return esEstadoFinal(estadoActual);
}

// Verificar si está vacío
bool AFD::estaVacio() const {
    return totalEstados == 0;
}

// Mostrar configuración del AFD
void AFD::mostrarConfiguracionAFD() const {
    cout << "\n" << string(60, '=') << endl;
    cout << "CONFIGURACION DEL AFD" << endl;
    cout << string(60, '=') << endl;
    
    cout << "\nALFABETO: { ";
    for (int i = 0; i < totalSimbolos; i++) {
        cout << alfabeto[i];
        if (i < totalSimbolos - 1) cout << ", ";
    }
    cout << " }\n";
    
    cout << "\nESTADOS: { ";
    for (int i = 0; i < totalEstados; i++) {
        cout << estados[i];
        if (i < totalEstados - 1) cout << ", ";
    }
    cout << " }\n";
    
    cout << "\nESTADO INICIAL: " << estadoInicial << endl;
    
    cout << "\nESTADOS FINALES: { ";
    for (int i = 0; i < totalEstadosFinales; i++) {
        cout << estadosFinales[i];
        if (i < totalEstadosFinales - 1) cout << ", ";
    }
    cout << " }\n";
    
    cout << string(60, '=') << "\n";
}

// Mostrar tabla de transiciones
void AFD::mostrarTablaTransiciones() const {
    if (estaVacio()) {
        cout << "AFD vacío.\n";
        return;
    }
    
    cout << "\n" << string(80, '=') << endl;
    cout << "TABLA DE TRANSICIONES" << endl;
    cout << string(80, '=') << endl;
    
    cout << "Estado ";
    for (int i = 0; i < totalSimbolos; i++) {
        cout << "| " << alfabeto[i] << " ";
    }
    cout << "|\n";
    cout << string(80, '-') << endl;
    
    for (int i = 0; i < totalEstados; i++) {
        cout << estados[i];
        int espacios = 10 - estados[i].length();
        for (int s = 0; s < espacios; s++) cout << " ";
        
        for (int j = 0; j < totalSimbolos; j++) {
            int siguiente = tablaTransiciones[i][j];
            if (siguiente == -1) {
                cout << "| - ";
            } else {
                cout << "| " << estados[siguiente] << " ";
            }
        }
        cout << "|\n";
    }
    
    cout << string(80, '=') << "\n";
}
