#ifndef STRUCTS_HPP
#define STRUCTS_HPP

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
}cadena;

typedef struct {
    pid_t pidServidor;
    pid_t pidCliente;
}pids;
typedef struct {
    cadena textos;
    pids procesos;
}datosCompartidos;


#endif