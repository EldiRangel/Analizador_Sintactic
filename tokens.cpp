#include "tokens.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

tokens::tokens() {
    tiposValidos[0] = "string";
    tiposValidos[1] = "integer";
    tiposValidos[2] = "boolean";
    tiposValidos[3] = "real";
    tiposValidos[4] = "char";
}

bool tokens::esTipoValido(string palabra) {
    for (int i = 0; i < 5; i++) {
        if (palabra == tiposValidos[i]) return true;
    }
    return false;
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
    archivo.close();
}

void tokens::revisarLinea(string linea, int numLinea) {
    if (linea.find("(*") == string::npos && linea.find("*)") != string::npos) {
        cout << "Linea " << numLinea << ": Error - Simbolo '*)' sin apertura." << endl;
    }

    size_t posDosPuntos = linea.find(":");
    if (posDosPuntos != string::npos) {
        string despuesDePuntos = linea.substr(posDosPuntos + 1);
        stringstream ss(despuesDePuntos);
        string palabra;
        
        if (ss >> palabra) {
            if (palabra.back() == ';') palabra.pop_back();
            
            if (!palabra.empty() && !esTipoValido(palabra) && 
                palabra.find("'") == string::npos && 
                palabra != ":=" && palabra != "=") {
                cout << "Linea " << numLinea << ": Error - Tipo '" << palabra << "' no reconocido." << endl;
            }
        }
    }

    size_t posIgual = linea.find("=");
    if (posIgual != string::npos) {
        bool tieneDosPuntosAntes = (posIgual > 0 && linea[posIgual-1] == ':');
        bool esComparacion = (linea.find("if") != string::npos || linea.find("<") != string::npos || linea.find(">") != string::npos);
        
        if (!tieneDosPuntosAntes && !esComparacion && linea.find("program") == string::npos) {
            cout << "Linea " << numLinea << ": Error - Asignacion invalida. Se esperaba ':='." << endl;
        }
    }

    if (linea.find(">") != string::npos || linea.find("<") != string::npos) {
        size_t posOp = linea.find_first_of("><");
        string resto = linea.substr(posOp + 1);
        stringstream ss(resto);
        string siguiente;
        if (!(ss >> siguiente) || siguiente == ")") {
            cout << "Linea " << numLinea << ": Error - Operador sin operando derecho." << endl;
        }
    }
}