#ifndef UTILITARIOS_HPP
#define UTILITARIOS_HPP


#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <limits>

using namespace std;

string obtenerPalabraDeArchivo();
int buscarApariciones(const string& palabra,char *palabraOculta,char letra);
bool esLetraIngresada(string& letras,char letraBuscada);
void reemplazar(string& palabraOculta,int pos);

bool validar(const string& letra);
bool esLetra(char letra);
void limpiarPantalla();

#endif
