#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits>
#include <vector>

#include "Proceso.cpp"
using namespace std;

void generarHijos(int numero,vector<int> padres,Proceso* hijo);

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3) {
        cerr << "Error, debe ingresar cantidad de niveles" << endl;
        return EXIT_FAILURE;
    }

    int numero = atoi(argv[1]);

    if (numero < 1)
        return EXIT_FAILURE;

    cout << "Generando " << numero << " niveles" << endl;

    vector<pid_t> padres = {};
    generarHijos(numero-1,padres,NULL);

    return EXIT_SUCCESS;
}

void generarHijos(int numero,vector<int> padres,Proceso* hijo)
{
    if (numero == 0){
        //cout<<"El proceso "<< getpid() << " es el ultimo posible." <<endl;
        
        //cout << "Esperando finalizacion..." << endl;
        sleep(5);

        hijo->mostrar();
        
        kill(getpid(),SIGTERM);
        delete(hijo);
    }

    padres.push_back(getpid());

    pid_t hijo1;
    Proceso* procesoHijo1;

    hijo1 = fork();
    if(hijo1==0)
        procesoHijo1 = new Proceso(getpid(),padres);

    pid_t hijo2 ;
    Proceso* procesoHijo2;

    if (hijo1 > 0){

        hijo2 = fork();
        //     cout<<"fork2 - PID: " << getppid() << "  " << getpid() <<endl;
    }
    else
    {
        generarHijos(numero - 1,padres,procesoHijo1);
    }

    if (hijo2 == 0)
    {
        Proceso* procesoHijo2 = new Proceso(getpid(),padres);
        generarHijos(numero - 1,padres,procesoHijo2);
    }

    wait(NULL);
    
    wait(NULL);

    if(hijo) {
        hijo->mostrar();
        delete(hijo);
    }

    kill(getpid(),SIGTERM);
}

/*

P0----*----*----  
P2    |    `----
P1     `---*----
P3         `----




Caso estándar
P0----*----------W-----X
P1     `----X...´

Caso demonio
P0----*---X
P1     `-------

Caso zombie
P0----*---------------X
P1     `----X..........______  <defunct>

*/