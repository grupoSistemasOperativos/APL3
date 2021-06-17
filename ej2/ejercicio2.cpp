#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include <chrono>

using namespace std;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

void* operaciones (void * datos);

typedef struct{
    int m;
    int n;
    int p;
    double *vecTiempos;
}t_datos;

int main(int argc, char **argv) {

    if (argc != 3) {
        cout << "Cantidad de parametros incorrecta. Uso: ./ejercicio2.exe n p o ./ejercicio2.exe -h" << endl;
        return EXIT_FAILURE;
    }

    if(argv[1] == "-h" || argv[1] == "--help") {   
        cout << "Para ejecutar este programa debe pasar por parametro la cantidad de iteraciones y el nivel de paralelismo." << endl;
        cout << "Uso: ./ejercicio2.exe cantidadIteraciones nivelParalelismo" << endl;
        cout << "Ejemplo: ./ejercicio2.exe 1000 4" << endl;
        return EXIT_SUCCESS;
    }

    int n = atoi(argv[1]);
    int p = atoi(argv[2]);

    if(n < 0) {
        cout << "La cantidad de iteraciones debe ser mayor a cero." << endl;
        return EXIT_FAILURE;
    }

    if(p < 1 || p > 8) {
        cout << "El nivel de paralelismo debe estar entre uno y ocho." << endl;
        return EXIT_FAILURE;
    }

    pthread_t hilos[p];
    t_datos datos[p];
    double tiempos[p];

    auto t1 = high_resolution_clock::now();
    for(int i = 0; i < p; i++) {
        datos[i].n = n;
        datos[i].p = p;
        datos[i].m = i+2;
        datos[i].vecTiempos = tiempos;
        if(pthread_create(&hilos[i], NULL, operaciones, &datos[i]) != 0)
            return EXIT_FAILURE; // QUIZAS HAY QUE MATARLOS ANTES DE HACER UN RETURN
    }

    for(int i = 0; i < p; i++)
        pthread_join(hilos[i], NULL);
    
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> tiempo_ms = t2 - t1;

    for(int i = 0; i < p; i++) {
        cout << "Tiempo Transcurrido por Hilo " << i+1 << endl;
        for(int j = i; j <= 7; j+=p) {
            cout << "El tiempo transcurrido en el ciclo " << j+2 << " fue de: " << tiempos[j] << " m/s" << endl;
        }
        cout << endl;
    }

    cout << "Tiempo final: " << tiempo_ms.count() << " m/s" << endl;
}

void* operaciones (void * datos) {
    t_datos dat = *((t_datos*) datos);
    int acumulador = dat.m;
    for(int i = dat.m; i <= 9; i+=dat.p) {
        auto t1 = high_resolution_clock::now();
        for(int j = 0; j < dat.n; j++) {
            acumulador *= dat.m;
            acumulador += acumulador;
            acumulador /= dat.n;
        }
        auto t2 = high_resolution_clock::now();
        duration<double, std::milli> tiempo_ms = t2 - t1;
        dat.vecTiempos[i-2] = tiempo_ms.count();
    }

    return NULL;
}