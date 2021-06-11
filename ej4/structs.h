#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
using namespace std;

typedef struct {
    char letrasIngresadas[100];
    char letra;
    char palabra[300];
    int intentos;
    int aciertos;
    char palabraOculta[300];
}datos;

#endif