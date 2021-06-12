#include "SharedMemory.h"

SharedMemory::SharedMemory() {
    this->idMemoria = shm_open(memoriaCompartida, O_CREAT | O_RDWR, 0600);
    //cout << idMemoria << endl;
    this->data = (datos *)mmap(NULL, sizeof(datos), PROT_READ | PROT_WRITE, MAP_SHARED, this->idMemoria, 0);
    this->procesos = (pids *)mmap(NULL, sizeof(pids), PROT_READ | PROT_WRITE, MAP_SHARED, this->idMemoria, 0);
    close(idMemoria);
}

SharedMemory::SharedMemory(string palabra,string palabraOculta) {
    this->idMemoria = shm_open(memoriaCompartida, O_CREAT | O_RDWR, 0600);
    ftruncate(this->idMemoria, sizeof(datos));

    //cout << this->idMemoria << endl;

    //this->data = new datos;

    this->data = (datos *)mmap(NULL, sizeof(datos), PROT_READ | PROT_WRITE, MAP_SHARED, this->idMemoria, 0);
    //close(idMemoria);
    this->procesos = (pids *)mmap(NULL, sizeof(pids), PROT_READ | PROT_WRITE, MAP_SHARED, this->idMemoria, 0);
    close(idMemoria);

    //this->procesos->pidServidor = pid;
    this->procesos->pidCliente = -1;
    //cout << "hola " << sizeof(datos) << endl;
    
    //cout << this->data << endl;

    //cout << this->data << endl;
    this->inicializarDatos(palabra,palabraOculta);

}


SharedMemory::~SharedMemory() {
    //close(this->idMemoria);
    
    // delete(this->data->palabra);
    // delete(this->data->letrasIngresadas);
    // delete(this->data->letra);

    munmap(this->data, sizeof(datos));
}

void SharedMemory::unMapear() {
    munmap(this->data, sizeof(datos));
}

void SharedMemory::inicializarDatos(string &palabra,string &palabraOculta) {
    
    strcpy(this->data->palabra,palabra.c_str());
    strcpy(this->data->palabraOculta,palabraOculta.c_str());
    *(this->data->letrasIngresadas) = '\0';
    this->data->intentos = 6;
    this->data->aciertos = 0;
    this->data->busquedaLetra = 0;
    this->data->fin=0;
}

datos* SharedMemory::getDatos() {
    return this->data;
}

int SharedMemory::getPidCliente() {
    return this->procesos->pidCliente;
}

int SharedMemory::getPidServidor() {
    return this->procesos->pidServidor;
}

void SharedMemory::setPidServidor(pid_t pid) {
    this->procesos->pidServidor = pid;
}

void SharedMemory::setPidCliente(pid_t pid) {
    this->procesos->pidCliente = pid;
}