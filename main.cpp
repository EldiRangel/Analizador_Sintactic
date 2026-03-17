#include "tokens.h"
#include <iostream>

using namespace std;

int main() {
    tokens analizador;

    string nombreArchivo = "codigo.txt";

    analizador.analizarArchivo(nombreArchivo);

    cout << "Analisis completado" << endl;

    return 0;
}