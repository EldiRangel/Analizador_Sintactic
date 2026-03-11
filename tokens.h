#ifndef TOKENS_H
#define TOKENS_H
#include <string>

struct Variable {
    std::string nombre;
    std::string tipo;
};

class tokens {
public:
    tokens();
    void analizarArchivo(std::string ruta);
private:
    std::string tiposValidos[5];
    Variable tablaSimbolos[100];
    int totalVariables;
    
    bool tieneProgram;
    bool enSeccionVar;
    int contadorBeginEnd;
    bool comentarioMultilinea;

    void revisarLinea(std::string linea, int numLinea);
    bool esTipoValido(std::string palabra);
    void registrarVariable(std::string nombre, std::string tipo, int linea);
    std::string obtenerTipoVariable(std::string nombre);
};

#endif
