
#include "tokens.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

tokens::tokens() {
    tiposValidos[0] = "string"; tiposValidos[1] = "integer";
    tiposValidos[2] = "boolean"; tiposValidos[3] = "real";
    tiposValidos[4] = "char";
    totalVariables = 0;
    tieneProgram = false;
    enSeccionVar = false;
    contadorBeginEnd = 0;
    comentarioMultilinea = false;
}

bool tokens::esTipoValido(string palabra) {
    for (int i = 0; i < 5; i++) {
        if (palabra == tiposValidos[i]) return true;
    }
    return false;
}

void tokens::registrarVariable(string nombre, string tipo, int linea) {
    for(int i = 0; i < totalVariables; i++) {
        if(tablaSimbolos[i].nombre == nombre) {
            cout << "Linea " << linea << ": Error - Variable '" << nombre << "' ya declarada." << endl;
            return;
        }
    }
    if(totalVariables < 100) {
        tablaSimbolos[totalVariables].nombre = nombre;
        tablaSimbolos[totalVariables].tipo = tipo;
        totalVariables++;
    }
}

string tokens::obtenerTipoVariable(string nombre) {
    for(int i = 0; i < totalVariables; i++) {
        if(tablaSimbolos[i].nombre == nombre) return tablaSimbolos[i].tipo;
    }
    return "desconocido";
}

void tokens::analizarArchivo(string ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return;
    string linea;
    int numLinea = 1;
    while (getline(archivo, linea)) {
        revisarLinea(linea, numLinea);
        numLinea++;
    }
    if (!tieneProgram) cout << "Error Global: Falta 'program'." << endl;
    if (contadorBeginEnd != 0) cout << "Error Global: Bloques begin/end desbalanceados." << endl;
    archivo.close();
}

void tokens::revisarLinea(string linea, int numLinea) {
    if (linea.empty()) return;

    // Manejo de comentarios multilinea (* ... *)
    if (linea.find("(*") != string::npos) comentarioMultilinea = true;
    if (comentarioMultilinea) {
        if (linea.find("*)") != string::npos) comentarioMultilinea = false;
        return; 
    }

    if (linea.find("program") != string::npos) tieneProgram = true;
    
    // Control de secciones
    if (linea.find("var") != string::npos) { enSeccionVar = true; return; }
    if (linea.find("begin") != string::npos) { enSeccionVar = false; contadorBeginEnd++; }
    if (linea.find("end") != string::npos) {
        contadorBeginEnd--;
        if (linea.find("end;") == string::npos && linea.find("end.") == string::npos)
            cout << "Linea " << numLinea << ": Error - Falta ';' o '.' despues de end." << endl;
    }

    // Registro de variables (Analisis Semantico)
    if (enSeccionVar && linea.find(":") != string::npos) {
        stringstream ss(linea);
        string nombre, puntos, tipo;
        ss >> nombre >> puntos >> tipo;
        if (tipo.back() == ';') tipo.pop_back();
        if (esTipoValido(tipo)) registrarVariable(nombre, tipo, numLinea);
        else cout << "Linea " << numLinea << ": Error - Tipo '" << tipo << "' invalido." << endl;
    }

    // Uso de variables y validacion de tipos
    size_t posAsignacion = linea.find(":=");
    if (posAsignacion != string::npos) {
        stringstream ss(linea.substr(0, posAsignacion));
        string varIzquierda;
        ss >> varIzquierda;
        
        string tipoIzquierdo = obtenerTipoVariable(varIzquierda);
        if (tipoIzquierdo == "desconocido") {
            cout << "Linea " << numLinea << ": Error - Variable '" << varIzquierda << "' no declarada." << endl;
        }

        // Check basico de punto y coma
        if (linea.back() != ';' && linea.find("begin") == string::npos && linea.find("end") == string::npos) {
            // Ignorar espacios en blanco al final
            size_t ultimo = linea.find_last_not_of(" ");
            if (linea[ultimo] != ';') cout << "Linea " << numLinea << ": Error - Falta ';'." << endl;
        }
    }
}