#include "tokens.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

tokens::tokens() {
    tieneProgram = false;
    tieneVar = false;
    contadorBeginEnd = 0;
    comentarioMultilinea = false;
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

    if (!tieneProgram) cout << "Error Estructural: No se encontro la palabra clave 'program'." << endl;
    if (contadorBeginEnd > 0) cout << "Error Estructural: Faltan " << contadorBeginEnd << " cierres de bloque 'end'." << endl;
    if (contadorBeginEnd < 0) cout << "Error Estructural: Hay 'end' de mas en el archivo." << endl;

    archivo.close();
}

void tokens::revisarLinea(string linea, int numLinea) {
    if (linea.empty()) return;

    if (linea.find("(*") != string::npos) comentarioMultilinea = true;
    if (comentarioMultilinea) {
        if (linea.find("*)") != string::npos) comentarioMultilinea = false;
        return;
    }

    if (linea.find("program") != string::npos) tieneProgram = true;
    if (linea.find("var") != string::npos) tieneVar = true;
    if (linea.find("begin") != string::npos) contadorBeginEnd++;
    
    if (linea.find("end") != string::npos) {
        contadorBeginEnd--;
        size_t posEnd = linea.find("end");
        if (linea.find(".", posEnd) == string::npos && linea.find(";", posEnd) == string::npos) {
            cout << "Linea " << numLinea << ": Error sintactico - 'end' debe finalizar con '.' o ';'." << endl;
        }
    }

    if (linea.find("if") != string::npos && linea.find("then") == string::npos) {
        cout << "Linea " << numLinea << ": Error sintactico - Sentencia 'if' sin su clausula 'then'." << endl;
    }

    if (linea.find(":=") == string::npos && linea.find("=") != string::npos) {
        if (linea.find("if") == string::npos && linea.find("program") == string::npos) {
            cout << "Linea " << numLinea << ": Error sintactico - Uso de '=' en lugar de ':=' para asignacion." << endl;
        }
    }

    size_t ultimo = linea.find_last_not_of(" \t\n\r");
    if (ultimo != string::npos) {
        char c = linea[ultimo];
        if (c != ';' && c != '.' && 
            linea.find("begin") == string::npos && 
            linea.find("var") == string::npos && 
            linea.find("then") == string::npos &&
            linea.find("else") == string::npos) {
            cout << "Linea " << numLinea << ": Error sintactico - Se esperaba ';' al final de la linea." << endl;
        }
    }
}