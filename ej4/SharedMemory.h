#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "bibliotecas.h"

#define memoriaCompartida "memoriaCompartida"

class SharedMemory {
    public:
        SharedMemory(string,string);
        SharedMemory();
        ~SharedMemory();
        datos* getDatos();
    private:
        int idMemoria;
        datos* data;
        void inicializarDatos(string&,string&);
        void copiarCadena(string&);
};

#endif