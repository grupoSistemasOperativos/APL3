#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits>

void generarHijos(int);

using namespace std;

int main(int argc, char **argv)
{
    int numero = *argv[1] - '0';
    
    if (argc < 2 || argc > 3)
        return EXIT_FAILURE;

    if (numero < 1)
        return EXIT_FAILURE;

    generarHijos(numero-1);

    return EXIT_SUCCESS;
}

void generarHijos(int numero)
{
    if (numero == 0){
        cout<<"paso por aca - pid : "<< getpid() <<endl;
        
        cout << "Presione enter para continuar... " << endl;
        cin.get();

        kill(getpid(),SIGKILL);
    }

    pid_t hijo1;
    
    

    hijo1 = fork();
    if(hijo1==0)
    cout<<"fork1- pid :"<< getpid() <<endl;
   

    pid_t hijo2 ;

    if (hijo2 > 0){

        hijo2 = fork();
        if(hijo2==0)
        cout<<"fork2 - PID" << getpid() <<endl;
    }
    else
    {
        generarHijos(numero - 1);
    }

    if (hijo2 == 0)
        generarHijos(numero - 1);

    cout << "esperado hijo 1" << endl;
    wait(NULL);
    kill(getpid(),SIGKILL);
    cout << "esperado hijo 2" << endl;
    wait(NULL);
    kill(getpid(),SIGKILL);
    cout<< "sigo con mi vida "<< endl;
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