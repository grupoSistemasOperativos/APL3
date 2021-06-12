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
        void unMapear();
        int getPidCliente();
        int getPidServidor();
        void setPidServidor(pid_t);
        void setPidCliente(pid_t);
    private:
        int idMemoria;
        datos* data;
        pids* procesos;
        void inicializarDatos(string&,string&);
        void copiarCadena(string&);
};

#endif