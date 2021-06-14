#include "bibliotecas.hpp"

datosCompartidos* obtenerDatosCompartidos();
bool inicializarMemoria();
void inicializarDatos(datosCompartidos* datos,const string* palabra);
string inicializar(int cant);

bool inicializarMemoria() {
    int idMemoria = shm_open(memoriaCompartida, O_RDWR | O_CREAT | O_EXCL, 0600);
    if(idMemoria < 0) {
        close(idMemoria);
        return false;
    }
    ftruncate(idMemoria,sizeof(datosCompartidos));
    //close(idMemoria);
    return true;
}

datosCompartidos* obtenerDatosCompartidos() {
    
    int idMemoria = shm_open(memoriaCompartida, O_CREAT | O_RDWR, 0600);
    // cout << idMemoria << endl;
    // if(idMemoria < 0)
    //     return NULL;
    datosCompartidos* datos = (datosCompartidos *)mmap(NULL, sizeof(datosCompartidos), PROT_READ | PROT_WRITE, MAP_SHARED, idMemoria, 0);

    //cout << "verificando si esta vacia " << datos->textos.palabra << " " << datos->textos.palabraOculta << " " << datos->textos.letrasIngresadas << endl;

    close(idMemoria);
    
    return datos;
}

void inicializarDatos(datosCompartidos* datos,const string* palabra) {
    
    //cout << "hola" << endl;
    memset(datos,'\0',sizeof(datosCompartidos));

    strcpy(datos->textos.palabra,palabra->c_str());
    //cout << "hola" << endl;
    strcpy(datos->textos.palabraOculta,inicializar(palabra->size()).c_str());

    *(datos->textos.letrasIngresadas) = '\0';

    datos->textos.intentos = 6;
    datos->textos.aciertos = 0;
    datos->textos.busquedaLetra = 0;
    datos->textos.fin = 0;
    datos->procesos.pidServidor = getpid();
    datos->procesos.pidCliente = -1;
}

void liberarMemoriaCompartida() {
    munmap(obtenerDatosCompartidos(),sizeof(datosCompartidos));
    //shm_unlink(memoriaCompartida);
}

string inicializar(int cant) {
    string palabraOculta = "";
    
    while(cant-- > 0) {
        palabraOculta += "_";
    }

    return palabraOculta;
}