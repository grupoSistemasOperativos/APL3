#include "validaciones.hpp"

bool validarEjecucionServidor() {
 
    return shm_open(memoriaCompartida, O_RDONLY, 0600) >= 0;
}

bool seEjecutaCliente() {
    return obtenerDatosCompartidos()->procesos.pidCliente >= 0;
}