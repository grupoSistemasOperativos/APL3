#include "memoria.hpp"

bool inicializarMemoria() {
    int idMemoria = shm_open(memoriaCompartida, O_RDWR | O_CREAT | O_EXCL, 0600);
    if(idMemoria < 0) {
        close(idMemoria);
        return false;
    }
    ftruncate(idMemoria,sizeof(memoriaCompartida));
    //close(idMemoria);
    return true;
}

memoria* obtenerDatosCompartidos() {
    
    int idMemoria = shm_open(memoriaCompartida, O_CREAT | O_RDWR, 0600);
    // cout << idMemoria << endl;
    // if(idMemoria < 0)
    //     return NULL;
    memoria* mem = (memoria*)mmap(NULL, sizeof(memoria), PROT_READ | PROT_WRITE, MAP_SHARED, idMemoria, 0);

    //cout << "verificando si esta vacia " << datos->textos.palabra << " " << datos->textos.palabraOculta << " " << datos->textos.letrasIngresadas << endl;

    close(idMemoria);
    
    return mem;
}

void inicializarDatos(memoria* datos,int largoPalabra) {
    
    //cout << "hola" << endl;
    memset(datos,'\0',sizeof(memoria));

    //cout << "hola" << endl;
    strcpy(datos->palabraOculta,inicializar(largoPalabra).c_str());

    datos->intentos = 6;
    datos->fin = 0;
    datos->procesos.pidServidor = getpid();
    datos->procesos.pidCliente = -1;
}

void liberarMemoriaCompartida() {
    munmap(obtenerDatosCompartidos(),sizeof(memoria));
    //shm_unlink(memoriaCompartida);
}

string inicializar(int cant) {
    string palabraOculta = "";
    
    while(cant-- > 0) {
        palabraOculta += "_";
    }

    return palabraOculta;
}