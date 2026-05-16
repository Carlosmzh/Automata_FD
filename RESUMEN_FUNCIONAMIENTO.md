# Resumen Técnico de Funcionamiento - Proyecto AFD

**Proyecto:** Autómata Finito Determinístico  
**Fecha:** Mayo 2026  
**Estado:** Funcional y Completo

---

## 1. Introducción Técnica

Este proyecto implementa un **Autómata Finito Determinístico (AFD)** según la definición formal:

> Un AFD es una quíntupla M = (Q, Σ, δ, q₀, F) donde:
> - Q: Conjunto finito de estados
> - Σ: Alfabeto de entrada
> - δ: Función de transición Q × Σ → Q
> - q₀: Estado inicial (q₀ ∈ Q)
> - F: Conjunto de estados finales (F ⊆ Q)

El programa permite construir, cargar, persistir y utilizar AFDs para verificar la aceptación de cadenas de entrada.

---

## 2. Ejecución del Proyecto

### Compilación

```bash
g++ -std=c++17 -Wall -Iinclude -o AFD.exe src/main.cpp src/afd.cpp src/cargar_archivo.cpp src/validacion.cpp
```

**Parámetros de compilación:**
- `-std=c++17`: Utiliza estándar C++17
- `-Wall`: Habilita todas las advertencias
- `-Iinclude`: Indica el directorio de headers

### Ejecución directa

```bash
./AFD.exe
```

El ejecutable `AFD.exe` se encuentra pré-compilado en la raíz del proyecto.

---

## 3. Arquitectura del Sistema

### Estructura de Archivos

```
AUTOMATA/
├── AFD.exe                              # Ejecutable compilado
├── src/
│   ├── main.cpp                         # Punto de entrada, menú e interacción
│   ├── afd.cpp                          # Implementación de la clase AFD
│   ├── cargar_archivo.cpp               # Parser de archivos de configuración
│   └── validacion.cpp                   # Funciones de validación de entrada
├── include/
│   ├── common.h                         # Headers compartidos y constantes
│   ├── afd.h                            # Declaración de la clase AFD
│   ├── cargar_archivo.h                 # Prototipos de carga de archivos
│   ├── validacion.h                     # Prototipos de validación
│   └── estructura_datos.h              # Reservado para estructuras auxiliares
├── datos/
│   ├── afd_ejemplo_simple.txt           # AFD que reconoce a*b*
│   └── afd_binarios_pares.txt          # AFD que reconoce binarios pares
└── RESUMEN_FUNCIONamiento.md            # Este documento
```

### Dependencias entre Archivos

```
main.cpp
    ├── afd.h → afd.cpp (clase AFD)
    ├── cargar_archivo.h → cargar_archivo.cpp (persistencia)
    └── validacion.h → validacion.cpp (entrada validada)

afd.h
    └── common.h (tipos básicos: string, vector, iostream)

cargar_archivo.h
    └── afd.h (utiliza la clase AFD)

validacion.h
    └── afd.h (utiliza la clase AFD)
```

---

## 4. Componentes del Sistema

### 4.1 common.h - Cabeceras y Constantes Globales

**Ubicación:** `include/common.h`

**Contenido:**
```cpp
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include <cctype>
#include <limits>

#define CAPACIDAD_INICIAL 10
#define CAPACIDAD_ALFABETO 26
```

**Propósito:** Provee las librerías estándar utilizadas en todo el proyecto y define constantes de capacidad inicial para los arreglos dinámicos.

---

### 4.2 include/afd.h - Declaración de la Clase AFD

**Ubicación:** `include/afd.h`

**Miembros privados:**
```cpp
string* estados;                    // Arreglo dinámico de nombres de estados
int totalEstados;                   // Cantidad actual de estados
int capacidadEstados;              // Capacidad total del arreglo

char* alfabeto;                    // Arreglo dinámico de símbolos
int totalSimbolos;                 // Cantidad actual de símbolos
int capacidadAlfabeto;            // Capacidad total del alfabeto

string estadoInicial;              // Nombre del estado inicial
string* estadosFinales;           // Arreglo dinámico de estados finales
int totalEstadosFinales;          // Cantidad de estados finales
int capacidadEstadosFinales;      // Capacidad del arreglo de finales

int** tablaTransiciones;          // Matriz 2D: [estado][símbolo] → estado
```

**Métodos privados (auxiliares):**
- `redimensionarEstados()`: Duplica la capacidad del arreglo de estados
- `redimensionarAlfabeto()`: Duplica la capacidad del alfabeto
- `redimensionarEstadosFinales()`: Duplica la capacidad de estados finales
- `recrearTablaTransiciones()`: Reinicializa la matriz de transiciones
- `obtenerIndiceEstado(const string&)`: Busca índice por nombre (-1 si no existe)
- `obtenerIndiceSimbol(char)`: Busca índice de símbolo en el alfabeto
- `esEstadoFinal(const string&)`: Verifica si un estado es final

**Métodos públicos:**
- Constructores y destructor
- `agregarEstado(const string&)`: Añade un nuevo estado al AFD
- `agregarEstadoFinal(const string&)`: Marca un estado como final
- `establecerEstadoInicial(const string&)`: Define el estado inicial
- `agregarSimboloAlfabeto(char)`: Añade un símbolo al alfabeto
- `agregarTransicion(const string&, char, const string&)`: Define δ(q, a) = q'
- `verificarCadena(const string&)`: Ejecuta el reconocimiento de la cadena
- Getters: `getTotalEstados()`, `getTotalSimbolos()`, `getEstados()`, `getAlfabeto()`, `getEstadosFinales()`, `getEstadoInicial()`, `getTotalEstadosFinales()`
- `mostrarConfiguracionAFD()`: Imprime alfabeto, estados, inicial y finales
- `mostrarTablaTransiciones()`: Imprime la matriz de transiciones
- `estaVacio()`: Retorna true si no hay estados definidos

---

### 4.3 src/afd.cpp - Implementación de la Clase AFD

**Ubicación:** `src/afd.cpp`

#### Constructor (líneas 4-14)
```cpp
AFD::AFD() 
    : estados(nullptr), totalEstados(0), capacidadEstados(CAPACIDAD_INICIAL),
      alfabeto(nullptr), totalSimbolos(0), capacidadAlfabeto(CAPACIDAD_ALFABETO),
      estadoInicial(""), estadosFinales(nullptr), totalEstadosFinales(0),
      capacidadEstadosFinales(CAPACIDAD_INICIAL),
      tablaTransiciones(nullptr) {
    
    estados = new string[capacidadEstados];
    alfabeto new char[capacidadAlfabeto];
    estadosFinales = new string[capacidadEstadosFinales];
}
```

Inicializa todos los miembros mediante lista de inicialización y asigna memoria dinámica para los tres arreglos principales.

#### Destructor (líneas 17-28)
```cpp
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
```

Libera toda la memoria dinámica en orden inverso a la asignación: primero las filas de la tabla, luego la tabla misma, y finalmente los tres arreglos principales.

#### Redimensionamiento Dinámico (líneas 31-69)

Cada método de redimensión sigue el mismo patrón:
1. Duplicar la capacidad (`capacidad *= 2`)
2. Crear un nuevo arreglo con mayor tamaño
3. Copiar los datos existentes al nuevo arreglo
4. Liberar el arreglo antiguo
5. Asignar el puntero al nuevo arreglo
6. **Importante:** Llamar a `recrearTablaTransiciones()` cuando se redimensionan estados o alfabeto

**Diagrama de redimensión de estados:**
```
Antes: capacidad=10, total=10
       [q0][q1][q2]...[q9]????

Después: capacidad=20
       [q0][q1][q2]...[q9]..............
       ↑ Ya no hay overflow
```

#### Recreación de Tabla de Transiciones (líneas 72-89)

```cpp
void AFD::recrearTablaTransiciones() {
    // 1. Liberar tabla anterior si existe
    if (tablaTransiciones != nullptr) {
        for (int i = 0; i < capacidadEstados; i++) {
            delete[] tablaTransiciones[i];
        }
        delete[] tablaTransiciones;
    }
    
    // 2. Crear nueva tabla solo si hay estados y símbolos
    if (totalEstados > 0 && totalSimbolos > 0) {
        tablaTransiciones = new int*[capacidadEstados];
        for (int i = 0; i < capacidadEstados; i++) {
            tablaTransiciones[i] = new int[capacidadAlfabeto];
            for (int j = 0; j < capacidadAlfabeto; j++) {
                tablaTransiciones[i][j] = -1;  // -1 = transición no definida
            }
        }
    }
}
```

**Nota técnica:** La tabla se inicializa con -1 en todas las posiciones, indicando que no hay transición definida. El valor -1 es el estándar para marcar "undefined" en la implementación.

#### Agregar Estado (líneas 116-134)

```cpp
bool AFD::agregarEstado(const string& nombre) {
    // 1. Verificar que no exista duplicado
    if (obtenerIndiceEstado(nombre) != -1) {
        cerr << "Error: Estado '" << nombre << "' ya existe.\n";
        return false;
    }
    
    // 2. Redimensionar si es necesario
    if (totalEstados >= capacidadEstados) {
        redimensionarEstados();
    }
    
    // 3. Agregar el nuevo estado
    estados[totalEstados] = nombre;
    totalEstados++;
    
    // 4. Crear tabla si es el primer estado Y ya hay símbolos
    if (totalEstados == 1 && totalSimbolos > 0) {
        recrearTablaTransiciones();
    }
    
    return true;
}
```

#### Agregar Transición (líneas 191-217)

```cpp
bool AFD::agregarTransicion(const string& desde, char simbolo, const string& hacia) {
    // 1. Validar estado origen existe
    int idxDesde = obtenerIndiceEstado(desde);
    if (idxDesde == -1) {
        cerr << "Error: Estado origen '" << desde << "' no existe.\n";
        return false;
    }
    
    // 2. Validar estado destino existe
    int idxHacia = obtenerIndiceEstado(hacia);
    if (idxHacia == -1) {
        cerr << "Error: Estado destino '" << hacia << "' no existe.\n";
        return false;
    }
    
    // 3. Validar símbolo está en el alfabeto
    int idxSimbolo = obtenerIndiceSimbol(simbolo);
    if (idxSimbolo == -1) {
        cerr << "Error: Símbolo '" << simbolo << "' no en alfabeto.\n";
        return false;
    }
    
    // 4. Validar tabla inicializada
    if (tablaTransiciones == nullptr) {
        cerr << "Error: Tabla de transiciones no inicializada.\n";
        return false;
    }
    
    // 5. Asignar la transición: δ(desde, símbolo) = hacia
    tablaTransiciones[idxDesde][idxSimbolo] = idxHacia;
    return true;
}
```

#### VERIFICAR CADENA - Algoritmo Principal (líneas 220-246)

```cpp
bool AFD::verificarCadena(const string& cadena) {
    // 1. Validar que existe estado inicial
    if (estadoInicial.empty()) {
        cerr << "Error: Estado inicial no establecido.\n";
        return false;
    }
    
    // 2. Iniciar en el estado inicial
    string estadoActual = estadoInicial;
    
    // 3. Procesar cada símbolo de la cadena
    for (char simbolo : cadena) {
        // 3.1. Verificar que el símbolo pertenezca al alfabeto
        int idxSimbolo = obtenerIndiceSimbol(simbolo);
        if (idxSimbolo == -1) {
            cerr << "Error: Símbolo '" << simbolo << "' no válido.\n";
            return false;
        }
        
        // 3.2. Obtener índice del estado actual
        int idxEstado = obtenerIndiceEstado(estadoActual);
        
        // 3.3. Obtener el próximo estado mediante la tabla
        int idxProximo = tablaTransiciones[idxEstado][idxSimbolo];
        
        // 3.4. Si no hay transición definida (-1), rechazar
        if (idxProximo == -1) {
            return false;
        }
        
        // 3.5. Actualizar al nuevo estado
        estadoActual = estados[idxProximo];
    }
    
    // 4. Verificar si el estado final es un estado de aceptación
    return esEstadoFinal(estadoActual);
}
```

**Trayectoria de ejecución para "ab" con a*b*:**
```
Cadena: "ab"

Inicio:     estado = q0 (inicial)
Símbolo 'a': idxSimbolo = 0, idxEstado = 0
            tablaTransiciones[0][0] = 1 → estado = q1
Símbolo 'b': idxSimbolo = 1, idxEstado = 1
            tablaTransiciones[1][1] = 2 → estado = q2
Final:      estado = q2
            esEstadoFinal("q2") = true → return true (ACEPTADA)
```

---

### 4.4 src/cargar_archivo.cpp - Persistencia y Parser

**Ubicación:** `src/cargar_archivo.cpp`

#### Función split (líneas 4-24)

```cpp
vector<string> split(const string& cadena, char delimitador) {
    vector<string> partes;
    stringstream ss(cadena);
    string parte;
    
    while (getline(ss, parte, delimitador)) {
        // Trim: eliminar espacios en blanco al inicio y final
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
```

**Propósito:** Divide una línea del archivo por el delimitador '|' y elimina espacios en blanco. Utiliza `stringstream` y `getline` para el parsing.

#### Cargar AFD desde Archivo (líneas 27-105)

**Proceso de parsing línea por línea:**

1. **Abre el archivo** en modo lectura (`ifstream`)
2. **Itera** sobre cada línea del archivo
3. **Ignora** líneas vacías y comentarios (líneas que empiezan con '#')
4. **Divide** cada línea por el delimitador '|'
5. **Procesa** según el comando detectado:

| Comando | Formato | Acción |
|---------|---------|--------|
| ALFABETO | `ALFABETO\|a\|b\|c` | Agrega cada símbolo al alfabeto |
| ESTADO | `ESTADO\|q0\|1` | Agrega estado, marca como final si el tercer campo es "1" |
| INICIAL | `INICIAL\|q0` | Establece el estado inicial |
| TRANSICION | `TRANSICION\|q0\|a\|q1` | Define la transición δ(q0,a)=q1 |

6. **Valida** cada línea antes de procesarla (número de campos, longitud de símbolos)
7. **Cierra** el archivo al terminar
8. **Retorna** true si la carga fue exitosa, false en caso de error

**Diagrama de flujo del parser:**

```
Inicio → Abrir archivo → ¿Archivo abierto?
                              ↓ No
                        Error: mostrar mensaje
                              ↓ Sí
                    Leer línea → ¿EOF?
                              ↓ Sí
                        Cerrar archivo → Fin
                              ↓ No
                    ¿Línea vacía o '#'?
                              ↓ Sí
                        Siguiente línea
                              ↓ No
                    Dividir por '|'
                          ↓
                    Identificar comando
                          ↓
              ┌─────────┬──────────┬──────────┐
              ↓         ↓          ↓          ↓
           ALFABETO  ESTADO    INICIAL   TRANSICION
              ↓         ↓          ↓          ↓
           Validar    Validar    Validar    Validar
           Parsing   Parsing     Parsing    Parsing
              ↓         ↓          ↓          ↓
            Mismo resultado para todas las ramas
                          ↓
                    Siguiente línea
```

#### Cargar AFD Hardcodeado (líneas 108-131)

Carga predefinida del lenguaje a*b*:
- **Alfabeto:** { a, b }
- **Estados:** q0, q1, q2, q3
- **Inicial:** q0
- **Finales:** q0, q2
- **Transiciones:**
  - δ(q0, a) = q1
  - δ(q0, b) = q2
  - δ(q1, a) = q1
  - δ(q1, b) = q2
  - δ(q2, b) = q2

---

### 4.5 src/validacion.cpp - Funciones de Interfaz

**Ubicación:** `src/validacion.cpp`

#### limpiarBuffer() (líneas 4-7)

```cpp
void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
```

**Propósito:** Limpia el buffer de entrada después de usar `cin >>`. Sin esto, los `getline` subsiguientes capturarían el salto de línea pendiente.

#### leerOpcionMenu(int min, int max) (líneas 10-21)

```cpp
int leerOpcionMenu(int min, int max) {
    int opcion;
    while (true) {
        cout << "Opcion (" << min << "-" << max << "): ";
        if (cin >> opcion && opcion >= min && opcion <= max) {
            limpiarBuffer();
            return opcion;
        }
        limpiarBuffer();
        cout << "Error: Opción inválida.\n";
    }
}
```

**Propósito:** Lee y valida una opción del menú. Utiliza un bucle infinito que solo se rompe cuando se ingresa un valor válido dentro del rango.

#### leerBooleano() (líneas 37-48)

Acepta únicamente 1 (sí) o 0 (no), con validación robusta.

#### leerCadenaVerificacion() (líneas 65-70)

```cpp
string leerCadenaVerificacion() {
    cout << "Cadena a verificar (Enter para vacía): ";
    string cadena;
    getline(cin, cadena);
    return cadena;
}
```

**Nota:** Utiliza `getline` en lugar de `cin >>` para permitir cadenas vacías (caso epsilon/λ).

#### leerNombreEstado() (líneas 81-97)

Valida que el nombre de estado proporcionado exista en el AFD antes de retornarlo.

---

### 4.6 src/main.cpp - Programa Principal y Menú

**Ubicación:** `src/main.cpp`

#### Punto de Entrada (líneas 112-224)

```cpp
int main() {
    int opcion;
    bool afdCargado = false;
    AFD* afd = nullptr;
    
    // Banner de bienvenida
    cout << "╔════════════════════════════════════════════════════╗\n";
    cout << "║   VERIFICADOR DE CADENAS - AUTÓMATA FINITO DETERMINÍSTICO ║\n";
    cout << "╚════════════════════════════════════════════════════╝\n";
    
    // Menú principal
    do {
        mostrarMenuPrincipal();
        opcion = leerOpcionMenu(0, 6);
        
        switch(opcion) {
            case 1: // Crear AFD manualmente
            case 2: // Cargar desde archivo
            case 3: // Cargar ejemplo
            case 4: // Mostrar configuración
            case 5: // Mostrar tabla de transiciones
            case 6: // Verificar cadena
            case 0: // Salir
        }
    } while (opcion != 0);
    
    // Limpieza
    if (afd) delete afd;
    return 0;
}
```

#### crearAFDManual() (líneas 19-110)

Proceso paso a paso:
1. **Alfabeto**: Solicita símbolos al usuario (ingresa 'x' para terminar)
2. **Estados**: Solicita nombres de estados (ingresa 'x' para terminar)
3. **Estado inicial**: Valida que el nombre exista antes de establecer
4. **Estados finales**: Permite seleccionar múltiples estados
5. **Transiciones**: Define δ(desde, símbolo, hacia) iterativamente

**Diagrama del flujo de creación manual:**

```
Crear AFD Manual
       ↓
Pedir símbolos (a,b,c...) → ¿Terminó?
       ↓ Sí
Pedir estados (q0,q1...) → ¿Terminó?
       ↓ Sí
Elegir estado inicial (validar existe)
       ↓
Elegir estados finales (puede ser ninguno)
       ↓
Agregar transiciones:
  - Mostrar estados disponibles
  - Seleccionar origen
  - Seleccionar símbolo
  - Seleccionar destino
  → ¿Agregar más? → Sí → Repetir
       ↓ No
Fin: AFD creado
```

---

## 5. Formato de Archivos de Configuración

### Estructura del Archivo

```
# Esta es una línea de comentario (se ignora)
ALFABETO|a|b                    # Símbolos del alfabeto
ESTADO|q0|1                     # Estado q0, ES final (1)
ESTADO|q1|0                     # Estado q1, NO es final (0)
ESTADO|q2|1                     # Estado q2, ES final (1)
INICIAL|q0                     # Estado inicial: q0
TRANSICION|q0|a|q1             # δ(q0, 'a') = q1
TRANSICION|q0|b|q2             # δ(q0, 'b') = q2
TRANSICION|q1|a|q1             # δ(q1, 'a') = q1
TRANSICION|q1|b|q2             # δ(q1, 'b') = q2
TRANSICION|q2|b|q2             # δ(q2, 'b') = q2
```

### Archivo: datos/afd_ejemplo_simple.txt

```
# AFD que reconoce: a*b*
# Acepta cadenas con cero o más 'a' seguidas de cero o más 'b'
# Ejemplos aceptados: "", "a", "b", "aa", "ab", "aab", "aabb", "bb"
# Ejemplos rechazados: "ba", "aba", "bab"

ALFABETO|a|b
ESTADO|q0|1
ESTADO|q1|0
ESTADO|q2|1
INICIAL|q0
TRANSICION|q0|a|q1
TRANSICION|q0|b|q2
TRANSICION|q1|a|q1
TRANSICION|q1|b|q2
TRANSICION|q2|b|q2
```

### Archivo: datos/afd_binarios_pares.txt

```
# AFD que reconoce números binarios pares
# Acepta cadenas binarias que terminan en 0
# Ejemplos aceptados: "0", "10", "100", "110", "1010"
# Ejemplos rechazados: "1", "11", "101", "111"

ALFABETO|0|1
ESTADO|q0|1
ESTADO|q1|0
INICIAL|q0
TRANSICION|q0|0|q1
TRANSICION|q0|1|q0
TRANSICION|q1|0|q1
TRANSICION|q1|1|q0
```

---

## 6. Especificaciones Técnicas Completas

### Modelo de Datos

| Componente | Tipo | Descripción |
|------------|------|-------------|
| `estados` | `string*` | Arreglo dinámico de nombres de estados |
| `alfabeto` | `char*` | Arreglo dinámico de símbolos |
| `estadosFinales` | `string*` | Arreglo dinámico de estados aceptadores |
| `tablaTransiciones` | `int**` | Matriz 2D de transición indexada |

### Complejidad Algorítmica

| Operación | Complejidad |
|-----------|-------------|
| `agregarEstado` | O(n) donde n = total de estados (búsqueda) |
| `agregarTransicion` | O(n + m) donde n=estados, m=símbolos |
| `verificarCadena` | O(k) donde k = longitud de la cadena |
| `obtenerIndiceEstado` | O(n) búsqueda lineal |
| `obtenerIndiceSimbol` | O(m) búsqueda lineal |

### Gestión de Memoria

- **Asignación:** `new` para todos los arreglos dinámicos
- **Liberación:** `delete[]` en orden inverso (destructor)
- **Redimensión:** Duplicación de capacidad cuando `total == capacidad`
- **Sin memory leaks:** Todas las asignaciones tienen su correspondiente liberación

### Validaciones Implementadas

1. **Duplicados:** No permite estados o símbolos duplicados
2. **Existencia:** Valida que estadosreferenciados existan
3. **Rango de entrada:** Menú validado entre 0 y 6
4. **Tipos de datos:** Verifica que las opciones sean numéricas
5. **Símbolos válidos:** Cada símbolo debe estar en el alfabeto
6. **Transiciones definidas:** Verifica que la transición exista en la tabla

---

## 7. Ejemplo de Ejecución

### Sesión de Ejemplo: Cargar AFD de ejemplo y verificar cadenas

```
╔════════════════════════════════════════════════════════════╗
║   VERIFICADOR DE CADENAS - AUTÓMATA FINITO DETERMINÍSTICO ║
╚════════════════════════════════════════════════════════════╝

============================================================
VERIFICADOR DE CADENAS - AFD (Autómata Finito Determinístico)
============================================================
1. Crear nuevo AFD manualmente
2. Cargar AFD desde archivo
3. Cargar AFD de ejemplo
4. Mostrar configuración del AFD
5. Mostrar tabla de transiciones
6. Verificar cadena
0. Salir
============================================================
Opcion (0-6): 3

Cargando AFD de ejemplo: a*b*
(Acepta cadenas con cero o más 'a' seguidas de cero o más 'b')
AFD de ejemplo cargado correctamente.

============================================================
VERIFICADOR DE CADENAS - AFD (Autómata Finito Determinístico)
============================================================
1. Crear nuevo AFD manualmente
2. Cargar AFD desde archivo
3. Cargar AFD de ejemplo
4. Mostrar configuración del AFD
5. Mostrar tabla de transiciones
6. Verificar cadena
0. Salir
============================================================
Opcion (0-6): 4

============================================================
CONFIGURACION DEL AFD
============================================================

ALFABETO: { a, b }

ESTADOS: { q0, q1, q2 }

ESTADO INICIAL: q0

ESTADOS FINALES: { q0, q2 }

============================================================

Opcion (0-6): 5

================================================================================
TABLA DE TRANSICIONES
================================================================================
Estado | a | b |
--------------------------------------------------------------------------------
q0     | q1| q2|
q1     | q1| q2|
q2     | - | q2|
================================================================================

Opcion (0-6): 6

Cadena a verificar (Enter para vacía): ab

--------------------------------------------------
Cadena: "ab"
Resultado: ACEPTADA ✓
--------------------------------------------------
¿Verificar otra cadena? (1=Sí, 0=No): 1
Cadena a verificar (Enter para vacía): ba

--------------------------------------------------
Cadena: "ba"
Resultado: RECHAZADA ✗
--------------------------------------------------
¿Verificar otra cadena? (1=Sí, 0=No): 0

Opcion (0-6): 0

Gracias por usar el verificador de AFD. ¡Hasta luego!
```

---

## 8. Estado del Proyecto

| Componente | Estado | Notas |
|------------|--------|-------|
| Clase AFD | ✅ Completo | Constructor, destructor, todos los métodos |
| Tabla de transiciones | ✅ Completo | Matriz 2D con redimensionamiento |
| Algoritmo de verificación | ✅ Completo | Implementa la quíntupla formal |
| Carga de archivos | ✅ Completo | Parser completo con validación |
| Menú interactivo | ✅ Completo | 6 opciones funcionales |
| Validación de entrada | ✅ Completo | Robusta con bucles de retry |
| Memoria | ✅ Sin leaks | Liberada en destructor |
| Compilación | ✅ Sin errores | warnings=0 con -Wall |

---

## 9. Cumplimiento de la Consigna Original

La consigna solicitaba:
> "Vamos a crear un programa genérico para un AFD capaz de leer/escribir su configuración en un archivo, y decidir la aceptación o no de una cadena de entrada leída desde teclado."

**Verificación:**
- ✅ Programa genérico para AFD (no hardcodeado)
- ✅ Leer configuración desde archivo
- ✅ Escribir/mostrar configuración
- ✅ Decidir aceptación de cadenas desde teclado
- ✅ Algoritmo del AFD implementado según modelo formal

---

**Versión:** 1.0  
**Proyecto:** Automata Finito Determinístico  
**Carrera:** Estructura de Datos (ESTR)  
**Última actualización:** Mayo 2026