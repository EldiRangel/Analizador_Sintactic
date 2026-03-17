#include "tokens.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

tokens::tokens() {
    tiposValidos[0] = "string";
    tiposValidos[1] = "integer";
    tiposValidos[2] = "boolean";
    tiposValidos[3] = "real";
    tiposValidos[4] = "char";
    tieneProgram = false;
    tieneVar = false;
    contadorBeginEnd = 0;
    errorEncontrado = false;
}

bool tokens::esTipoValido(string palabra) {
    for (int i = 0; i < 5; i++) {
        if (palabra == tiposValidos[i]) return true;
    }
    return false;
}

void tokens::analizarArchivo(const string& ruta) {
    ifstream archivo(ruta.c_str());
    if (!archivo.is_open()) return;

    string linea, ultimaLinea;
    int numLinea = 1;
    while (getline(archivo, linea)) {
        if (!linea.empty() && linea.find_first_not_of(" \t\n\r") != string::npos) {
            ultimaLinea = linea;
        }
        revisarLinea(linea, numLinea);
        if (errorEncontrado) break;
        numLinea++;
    }

    if (!errorEncontrado) {
        if (!tieneProgram) {
            cout << "Error Global: Falta 'program'." << endl;
            errorEncontrado = true;
        }
        if (ultimaLinea.find("end.") == string::npos) {
            cout << "Error Global: El programa debe terminar con 'end.' (punto final)." << endl;
            errorEncontrado = true;
        }
        if (contadorBeginEnd != 0) {
            cout << "Error Global: Bloques desbalanceados." << endl;
            errorEncontrado = true;
        }
    }

    archivo.close();
}

void tokens::revisarLinea(string linea, int numLinea) {
    if (errorEncontrado) return;
    if (linea.empty()) return;

    stringstream ss(linea);
    string palabra;
    while (ss >> palabra) {
        string limpia = "";
        bool tieneLetras = false;
        
        // Extrae solo caracteres alfanuméricos y detecta si hay letras
        for(size_t i = 0; i < palabra.size(); i++) {
            char c = palabra[i];
            if(isalnum(c)) {
                limpia += c;
                if(isalpha(c)) tieneLetras = true;
            }
        }

        if (limpia.empty()) continue;

        // Si no tiene letras, se ignora
        if (!tieneLetras) continue;

        // Registro de estados originales
        if (limpia == "program") tieneProgram = true;
        if (limpia == "var") tieneVar = true;
        if (limpia == "begin") contadorBeginEnd++;
        if (limpia == "end") contadorBeginEnd--;

        // si no es reservada, ni tipo, ni variable
        
        if (limpia != "program" && limpia != "var" && limpia != "begin" && limpia != "end" &&
            limpia != "if" && limpia != "then" && limpia != "else" && limpia != "Proyecto" &&
            limpia != "writeln" && limpia != "readln" &&
            !esTipoValido(limpia) && limpia != "a" && limpia != "b" && limpia != "resultado") {
            
            cout << "Linea " << numLinea << ": Error sintactico - Palabra '" << limpia << "' invalida." << endl;
            errorEncontrado = true;
            return;
        }
    }

    // Validación de Asignación
    if (linea.find("=") != string::npos && linea.find(":=") == string::npos) {
       
        if (linea.find("if") == string::npos && linea.find("program") == string::npos && linea.find("then") == string::npos) {
            cout << "Linea " << numLinea << ": Error - Use ':=' para asignar." << endl;
            errorEncontrado = true;
            return;
        }
    }

    size_t ultimo = linea.find_last_not_of(" \t\n\r");
    if (ultimo != string::npos) {
        char c = linea[ultimo];
        
        if (c != ';' && c != '.' && 
            linea.find("begin") == string::npos && 
            linea.find("var") == string::npos && 
            linea.find("then") == string::npos &&
            linea.find("else") == string::npos &&
            linea.find("end") == string::npos) { 
            
            cout << "Linea " << numLinea << ": Error sintactico - Falta ';' al final." << endl;
            errorEncontrado = true;
            return;
        }
    }
}