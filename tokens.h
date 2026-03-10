#ifndef TOKENS_H
#define TOKENS_H
#include <string>

class tokens {
public:
    tokens();
    void analizarArchivo(std::string ruta);
private:
    std::string tiposValidos[5];
    void revisarLinea(std::string linea, int numLinea);
    bool esTipoValido(std::string palabra);
};

#endif