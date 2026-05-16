#include "../include/cargar_archivo.h"

// Funcion para dividir cadena por delimitador
vector<string> split(const string& cadena, char delimitador) {
    vector<string> partes;
    stringstream ss(cadena);
    string parte;
    
    while (getline(ss, parte, delimitador)) {
        // Trim espacios en blanco al inicio
        size_t inicio = parte.find_first_not_of(" \t");
        size_t fin = parte.find_last_not_of(" \t");
        
        if (inicio != string::npos) {
            parte = parte.substr(inicio, fin - inicio + 1);
        } else {
            parte = "";
        }
        
        partes.push_back(parte);
    }
    
    return partes;
}

// Cargar AFD desde archivo
bool cargarAFDDesdeArchivo(AFD& afd, const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir '" << nombreArchivo << "'\n";
        return false;
    }
    
    string linea;
    int numeroLinea = 0;
    
    while (getline(archivo, linea)) {
        numeroLinea++;
        
        // Ignorar lineas vacias y comentarios
        if (linea.empty() || linea[0] == '#') continue;
        
        vector<string> partes = split(linea, '|');
        if (partes.empty()) continue;
        
        string comando = partes[0];
        
        // Comando ALFABETO
        if (comando == "ALFABETO") {
            for (size_t i = 1; i < partes.size(); i++) {
                if (partes[i].length() != 1) {
                    cerr << "Error linea " << numeroLinea << ": Simbolo invalido\n";
                    archivo.close();
                    return false;
                }
                afd.agregarSimboloAlfabeto(partes[i][0]);
            }
        }
        // Comando ESTADO
        else if (comando == "ESTADO") {
            if (partes.size() != 3) {
                cerr << "Error línea " << numeroLinea << ": Formato: ESTADO|nombre|esFinal\n";
                archivo.close();
                return false;
            }
            afd.agregarEstado(partes[1]);
            if (partes[2] == "1") {
                afd.agregarEstadoFinal(partes[1]);
            }
        }
        // Comando INICIAL
        else if (comando == "INICIAL") {
            if (partes.size() != 2) {
                cerr << "Error linea " << numeroLinea << ": Formato: INICIAL|nombre\n";
                archivo.close();
                return false;
            }
            afd.establecerEstadoInicial(partes[1]);
        }
        // Comando TRANSICION
        else if (comando == "TRANSICION") {
            if (partes.size() != 4) {
                cerr << "Error linea " << numeroLinea << ": Formato: TRANSICION|origen|simbolo|destino\n";
                archivo.close();
                return false;
            }
            if (partes[2].length() != 1) {
                cerr << "Error línea " << numeroLinea << ": Símbolo inválido\n";
                archivo.close();
                return false;
            }
            afd.agregarTransicion(partes[1], partes[2][0], partes[3]);
        }
        else {
            cerr << "Error linea " << numeroLinea << ": Comando '" << comando << "' desconocido\n";
            archivo.close();
            return false;
        }
    }
    
    archivo.close();
    cout << "\nAFD cargado correctamente desde '" << nombreArchivo << "'.\n";
    return true;
}

// Cargar AFD de ejemplo hardcodeado
void cargarAFDHardcodeado(AFD& afd) {
    cout << "\nCargando AFD de ejemplo: a*b*\n";
    cout << "(Acepta cadenas con cero o mas 'a' seguidas de cero o mas 'b')\n";
    
    afd.agregarSimboloAlfabeto('a');
    afd.agregarSimboloAlfabeto('b');
    
    afd.agregarEstado("q0");
    afd.agregarEstado("q1");
    afd.agregarEstado("q2");
    
    afd.agregarEstadoFinal("q0");
    afd.agregarEstadoFinal("q2");
    
    afd.establecerEstadoInicial("q0");
    
    afd.agregarTransicion("q0", 'a', "q1");
    afd.agregarTransicion("q0", 'b', "q2");
    afd.agregarTransicion("q1", 'a', "q1");
    afd.agregarTransicion("q1", 'b', "q2");
    afd.agregarTransicion("q2", 'b', "q2");
    
    cout << "AFD de ejemplo cargado correctamente.\n";
}
