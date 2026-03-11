#ifndef TOKENS_H
#define TOKENS_H
#include <string>

class tokens {
public:
    tokens();
    void analizarArchivo(std::string ruta);
private:
    bool tieneProgram;
    bool tieneVar;
    int contadorBeginEnd;
    bool comentarioMultilinea;
    
    void revisarLinea(std::string linea, int numLinea);
};

#endif