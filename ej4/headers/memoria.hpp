#ifndef MEMORIA_HPP
#define MEMORIA_HPP

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <limits>
#include <cstring>
#include <semaphore.h>
#include <signal.h>

#include "structs.hpp"

#define memoriaCompartida "memoriaCompartida"

memoria* obtenerDatosCompartidos();
bool inicializarMemoria();
void inicializarDatos(memoria*,int);
string inicializar(int);
void liberarMemoriaCompartida();

#endif