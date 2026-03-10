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
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo " << ruta << endl;
        return;
    }

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
        cout << "Linea " << numLinea << ": Error - Cierre '*)' sin apertura." << endl;
    }

    if (linea.find("(") != string::npos && linea.find("*)") != string::npos && linea.find("(*") == string::npos) {
         cout << "Linea " << numLinea << ": Error - Se detecto '(' pero se esperaba '(*' para el comentario." << endl;
    }

    size_t posDosPuntos = linea.find(":");
    if (posDosPuntos != string::npos) {
        stringstream ss(linea.substr(posDosPuntos + 1));
        string tipo;
        ss >> tipo;
        
        if (!tipo.empty() && tipo.back() == ';') tipo.pop_back();

        if (tipo == "strg") {
            cout << "Linea " << numLinea << ": Error lexico - 'strg' no es un tipo valido. Sugerencia: 'string'." << endl;
        } else if (!tipo.empty() && !esTipoValido(tipo) && tipo.find("'") == string::npos && tipo.find("=") == string::npos) {
            // Analisis de tipos desconocidos
        }
    }

    size_t posIgual = linea.find("=");
    if (posIgual != string::npos) {
        if (posIgual == 0 || linea[posIgual - 1] != ':') {
            if (linea.find("if") == string::npos && linea.find("program") == string::npos && linea.find(">") == string::npos && linea.find("<") == string::npos) {
                cout << "Linea " << numLinea << ": Error sintactico - Asignacion invalida. Se esperaba ':='." << endl;
            }
        }
    }

    if (linea.find(">") != string::npos || linea.find("<") != string::npos) {
        size_t posOp = linea.find_first_of("><");
        if (posOp + 1 < linea.length() && linea[posOp + 1] == ')') {
            cout << "Linea " << numLinea << ": Error sintactico - Falta operando despues del operador relacional." << endl;
        }
    }
}