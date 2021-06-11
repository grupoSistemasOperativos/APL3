#include "bibliotecas.h"

//bool validar(const string& letra);

int main() {
    SharedMemory sm;
    string letra;
    sem_t *semaforo = sem_open("empezarJuego", O_CREAT, 0600, 0);

    datos *datosJuego = sm.getDatos();

    sem_post(semaforo);

    return 0;
}

