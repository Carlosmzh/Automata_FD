#include "../include/afd.h"
#include "../include/cargar_archivo.h"
#include "../include/validacion.h"

void mostrarMenuPrincipal() {
    cout << "\n" << string(60, '=') << endl;
    cout << "VERIFICADOR DE CADENAS - AFD (Autómata Finito Determinístico)" << endl;
    cout << string(60, '=') << endl;
    cout << "1. Crear nuevo AFD manualmente\n";
    cout << "2. Cargar AFD desde archivo\n";
    cout << "3. Cargar AFD de ejemplo\n";
    cout << "4. Mostrar configuración del AFD\n";
    cout << "5. Mostrar tabla de transiciones\n";
    cout << "6. Verificar cadena\n";
    cout << "0. Salir\n";
    cout << string(60, '=') << endl;
}

void crearAFDManual(AFD& afd) {
    cout << "\n--- CREAR AFD MANUALMENTE ---\n";
    
    // Agregar símbolos al alfabeto
    cout << "\nPaso 1: Agregar símbolos al alfabeto\n";
    cout << "Ingrese 'x' cuando termine\n";
    while (true) {
        cout << "Símbolo: ";
        string input;
        cin >> input;
        limpiarBuffer();
        
        if (input == "x" || input == "X") break;
        if (input.length() != 1) {
            cout << "Error: Ingrese un solo carácter.\n";
            continue;
        }
        afd.agregarSimboloAlfabeto(input[0]);
    }
    
    if (afd.getTotalSimbolos() == 0) {
        cout << "Error: Debe agregar al menos un símbolo.\n";
        return;
    }
    
    // Agregar estados
    cout << "\nPaso 2: Agregar estados\n";
    cout << "Ingrese 'x' cuando termine\n";
    while (true) {
        cout << "Nombre del estado: ";
        string nombre;
        cin >> nombre;
        limpiarBuffer();
        
        if (nombre == "x" || nombre == "X") break;
        afd.agregarEstado(nombre);
    }
    
    if (afd.getTotalEstados() == 0) {
        cout << "Error: Debe agregar al menos un estado.\n";
        return;
    }
    
    // Establecer estado inicial
    cout << "\nPaso 3: Establecer estado inicial\n";
    mostrarEstados(&afd);
    string estadoInicial = leerNombreEstado(&afd, "Estado inicial");
    afd.establecerEstadoInicial(estadoInicial);
    
    // Agregar estados finales
    cout << "\nPaso 4: Marcar estados finales\n";
    while (true) {
        mostrarEstados(&afd);
        string estadoFinal = leerNombreEstado(&afd, "Estado final (o 'x' para terminar)");
        if (estadoFinal == "x" || estadoFinal == "X") break;
        afd.agregarEstadoFinal(estadoFinal);
    }
    
    // Agregar transiciones
    cout << "\nPaso 5: Agregar transiciones\n";
    cout << "Formato: origen - símbolo - destino\n";
    cout << "Ingrese 'x' cuando termine\n\n";
    
    while (true) {
        mostrarEstados(&afd);
        string origen = leerNombreEstado(&afd, "Estado origen (o 'x' para terminar)");
        if (origen == "x" || origen == "X") break;
        
        cout << "Símbolos disponibles: { ";
        for (int i = 0; i < afd.getTotalSimbolos(); i++) {
            cout << afd.getAlfabeto()[i];
            if (i < afd.getTotalSimbolos() - 1) cout << ", ";
        }
        cout << " }\n";
        
        string simboloStr;
        cout << "Símbolo: ";
        cin >> simboloStr;
        limpiarBuffer();
        
        if (simboloStr.length() != 1) {
            cout << "Error: Ingrese un solo símbolo.\n";
            continue;
        }
        
        string destino = leerNombreEstado(&afd, "Estado destino");
        afd.agregarTransicion(origen, simboloStr[0], destino);
        cout << "Transición agregada.\n\n";
    }
    
    cout << "\nAFD creado correctamente.\n";
}

int main() {
    int opcion;
    bool afdCargado = false;
    AFD* afd = nullptr;
    
    cout << "\n╔════════════════════════════════════════════════════════════╗\n";
    cout << "║                                                            ║\n";
    cout << "║   VERIFICADOR DE CADENAS - AUTÓMATA FINITO DETERMINÍSTICO ║\n";
    cout << "║                                                            ║\n";
    cout << "╚════════════════════════════════════════════════════════════╝\n";
    
    do {
        mostrarMenuPrincipal();
        opcion = leerOpcionMenu(0, 6);
        
        switch(opcion) {
            case 1: {
                // Crear nuevo AFD manualmente
                if (afd) delete afd;
                afd = new AFD();
                crearAFDManual(*afd);
                afdCargado = !afd->estaVacio();
                break;
            }
            
            case 2: {
                // Cargar AFD desde archivo
                if (!afdCargado) {
                    afd = new AFD();
                    afdCargado = true;
                } else {
                    delete afd;
                    afd = new AFD();
                }
                
                string ruta;
                cout << "\nRuta del archivo (default: datos/afd_ejemplo_simple.txt): ";
                cin.ignore();
                getline(cin, ruta);
                
                if (ruta.empty()) {
                    ruta = "datos/afd_ejemplo_simple.txt";
                }
                
                cargarAFDDesdeArchivo(*afd, ruta);
                break;
            }
            
            case 3: {
                // Cargar AFD de ejemplo
                if (afd) delete afd;
                afd = new AFD();
                cargarAFDHardcodeado(*afd);
                afdCargado = true;
                break;
            }
            
            case 4: {
                // Mostrar configuración
                if (!afdCargado || afd->estaVacio()) {
                    cout << "\nError: Carga un AFD primero.\n";
                    break;
                }
                afd->mostrarConfiguracionAFD();
                break;
            }
            
            case 5: {
                // Mostrar tabla de transiciones
                if (!afdCargado || afd->estaVacio()) {
                    cout << "\nError: Carga un AFD primero.\n";
                    break;
                }
                afd->mostrarTablaTransiciones();
                break;
            }
            
            case 6: {
                // Verificar cadena
                if (!afdCargado || afd->estaVacio()) {
                    cout << "\nError: Carga un AFD primero.\n";
                    break;
                }
                
                bool seguirVerificando = true;
                while (seguirVerificando) {
                    string cadena = leerCadenaVerificacion();
                    bool resultado = afd->verificarCadena(cadena);
                    
                    cout << "\n" << string(50, '-') << endl;
                    cout << "Cadena: \"" << cadena << "\"\n";
                    if (resultado) {
                        cout << "Resultado: ACEPTADA ✓\n";
                    } else {
                        cout << "Resultado: RECHAZADA ✗\n";
                    }
                    cout << string(50, '-') << "\n";
                    
                    seguirVerificando = leerBooleano("¿Verificar otra cadena?");
                }
                break;
            }
            
            case 0: {
                cout << "\nGracias por usar el verificador de AFD. ¡Hasta luego!\n\n";
                break;
            }
        }
    } while (opcion != 0);
    
    if (afd) delete afd;
    return 0;
}
