#include "SharedMemory.h"

SharedMemory::SharedMemory() {
    this->idMemoria = shm_open(memoriaCompartida, O_CREAT | O_RDWR, 777);

    this->data = (datos *)mmap(NULL, sizeof(datos), PROT_READ | PROT_WRITE, MAP_SHARED, this->idMemoria, 0);

    //cout << "prueba" << sizeof(datos) << endl;
    //close(idMemoria);
    //this->inicializarDatos(palabra,palabraOculta);
}

SharedMemory::SharedMemory(string palabra,string palabraOculta) {
    this->idMemoria = shm_open(memoriaCompartida, O_CREAT | O_RDWR, 777);
    ftruncate(this->idMemoria, sizeof(datos));

    //cout << this->idMemoria << endl;

    //this->data = new datos;

    this->data = (datos *)mmap(NULL, sizeof(datos), PROT_READ | PROT_WRITE, MAP_SHARED, this->idMemoria, 0);
    close(idMemoria);

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

void SharedMemory::inicializarDatos(string &palabra,string &palabraOculta) {
    
    strcpy(this->data->palabra,palabra.c_str());
    strcpy(this->data->palabraOculta,palabraOculta.c_str());
    *(this->data->letrasIngresadas) = '\0';
    this->data->intentos = 6;
    this->data->aciertos = 0;
}

datos* SharedMemory::getDatos() {
    return this->data;
}