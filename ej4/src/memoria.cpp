#include "../headers/memoria.hpp"

bool inicializarMemoria() {
    int idMemoria = shm_open(memoriaCompartida, O_RDWR | O_CREAT | O_EXCL, 0600);
    if(idMemoria < 0) {
        close(idMemoria);
        return false;
    }
    ftruncate(idMemoria,sizeof(memoriaCompartida));

    return true;
}

memoria* obtenerDatosCompartidos() {
    
    int idMemoria = shm_open(memoriaCompartida, O_CREAT | O_RDWR, 0600);

    memoria* mem = (memoria*)mmap(NULL, sizeof(memoria), PROT_READ | PROT_WRITE, MAP_SHARED, idMemoria, 0);

    close(idMemoria);
    
    return mem;
}

void inicializarDatos(memoria* datos,int largoPalabra) {
    
    memset(datos,'\0',sizeof(memoria));

    strcpy(datos->palabraOculta,inicializar(largoPalabra).c_str());

    datos->intentos = 6;
    datos->fin = 0;
    datos->procesos.pidServidor = getpid();
    datos->procesos.pidCliente = -1;
}

void liberarMemoriaCompartida() {
    munmap(obtenerDatosCompartidos(),sizeof(memoria));
}

string inicializar(int cant) {
    string palabraOculta = "";
    
    while(cant-- > 0) {
        palabraOculta += "_";
    }

    return palabraOculta;
}