#ifndef TOKENS_H
#define TOKENS_H

#include <string>

using namespace std;

class tokens {
private:
    string tiposValidos[5];
    bool tieneProgram;
    bool tieneVar;
    int contadorBeginEnd;

    // Métodos auxiliares de validación
    bool esTipoValido(string palabra);
    bool esPalabraReservada(string palabra);

public:
    tokens();
    void analizarArchivo(const string& ruta); // Cambiado para que coincida con la implementación ruta); // Cambiado para que coincida con la implementación
    void revisarLinea(string linea, int numLinea);
};

#endif