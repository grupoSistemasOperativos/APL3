#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
using namespace std;

typedef struct {
    char letrasIngresadas[100];
    char palabraOculta[300];
    char palabra[300];
    char letra;
    int intentos;
    int aciertos;
    int busquedaLetra;
    int fin;
}datos;

typedef struct {
    pid_t pidServidor;
    pid_t pidCliente;
}pids;

#endif