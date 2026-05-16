#include "../include/validacion.h"

// Limpiar buffer de entrada
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Leer opcion del menu
int leerOpcionMenu(int min, int max) {
    int opcion;
    while (true) {
        cout << "Opcion (" << min << "-" << max << "): ";
        if (cin >> opcion && opcion >= min && opcion <= max) {
            limpiarBuffer();
            return opcion;
        }
        limpiarBuffer();
        cout << "Error: Opcion invalida.\n";
    }
}

// Leer número entero
int leerEntero(int min) {
    int valor;
    while (true) {
        if (cin >> valor && valor >= min) {
            limpiarBuffer();
            return valor;
        }
        limpiarBuffer();
        cout << "Error: Ingrese numero >= " << min << ": ";
    }
}

// Leer booleano
bool leerBooleano(const string& contexto) {
    int valor;
    while (true) {
        cout << contexto << " (1=Si, 0=No): ";
        if (cin >> valor && (valor == 0 || valor == 1)) {
            limpiarBuffer();
            return valor == 1;
        }
        limpiarBuffer();
        cout << "Error: Ingrese 1 o 0.\n";
    }
}

// Leer string no vacío
string leerStringNoVacio(const string& campo) {
    string valor;
    while (true) {
        cout << campo << ": ";
        if (cin >> valor && !valor.empty()) {
            limpiarBuffer();
            return valor;
        }
        limpiarBuffer();
        cout << "Error: Ingrese un valor.\n";
    }
}

// Leer cadena para verificación
string leerCadenaVerificacion() {
    cout << "Cadena a verificar (Enter para vacia): ";
    string cadena;
    getline(cin, cadena);
    return cadena;
}

// Obtener indice de estado
int obtenerIndiceEstado(AFD* afd, const string& nombre) {
    for (int i = 0; i < afd->getTotalEstados(); i++) {
        if (afd->getEstados()[i] == nombre) return i;
    }
    return -1;
}

// Leer nombre de estado
string leerNombreEstado(AFD* afd, const string& contexto) {
    string nombre;
    while (true) {
        cout << contexto << ": ";
        if (cin >> nombre && !nombre.empty()) {
            limpiarBuffer();
            if (obtenerIndiceEstado(afd, nombre) != -1) {
                return nombre;
            }
            cout << "Error: Estado no existe.\n";
            mostrarEstados(afd);
        } else {
            limpiarBuffer();
            cout << "Error: Nombre invalido.\n";
        }
    }
}

// Mostrar estados disponibles
void mostrarEstados(AFD* afd) {
    cout << "Estados disponibles: { ";
    for (int i = 0; i < afd->getTotalEstados(); i++) {
        cout << afd->getEstados()[i];
        if (i < afd->getTotalEstados() - 1) cout << ", ";
    }
    cout << " }\n";
}
