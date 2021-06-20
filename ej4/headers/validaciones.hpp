#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

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

#include "memoria.hpp"

bool validarEjecucionServidor();
bool seEjecutaCliente();

#endif