#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <iostream>
using namespace std;


typedef struct {
    pid_t pidServidor;
    pid_t pidCliente;
}pids;
typedef struct {
    char palabraOculta[300];
    int intentos;
    int fin;
    char letraIngresada;
    int resultadoBusqueda;
    pids procesos;
}memoria;

#endif