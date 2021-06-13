#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits>
#include <vector>

//void generarHijos(int, vector <int> );

using namespace std;
void generarHijos(int numero,vector<int> padres)
{
    if (numero == 0){
        cout<< numero <<" El proceso "<< getpid() << " es el ultimo posible." <<endl;
        
        cout << "Presione enter para continuar... " << endl;
        sleep(5);
        //cin.get();
        cout << "Proceso " << getpid() << " Pid: ";

        for (size_t i = 0; i < padres.size(); i++)
        {
            cout << padres[i] << ", ";
        }
        cout << endl;
        kill(getpid(),SIGTERM);
    }

    pid_t hijo1;
    
    
    //padres.push_back((int)getpid());
    // cout << "hola "<< endl;
    // for (size_t i = 0; i < padres.size(); i++)
    // {
    //     cout << padres[i] << ", ";
    // }

    hijo1 = fork();
    // if(hijo1==0)
    // cout<<"fork1- PID: "<< getppid() << "  " << getpid() <<endl;
   

    pid_t hijo2 ;

    if (hijo1 > 0){
        hijo2 = fork();
        // if(hijo2 == 0)
        //     cout<<"fork2 - PID: " << getppid() << "  " << getpid() <<endl;
    }
    else
    {
        padres.push_back((int)getppid());
        generarHijos(numero - 1,padres);
    }

    if (hijo2 == 0) {
        padres.push_back((int)getppid());
        generarHijos(numero - 1,padres);
    }

    sleep(10);
    waitpid(hijo1,NULL,0);
    waitpid(hijo2,NULL,0);
    cout << "Proceso " << getpid() << "Pid: ";

    for (size_t i = 0; i < padres.size(); i++)
    {
        cout << padres[i] << ", ";
    }

    cout << endl;

    kill(getpid(),SIGTERM);
}

int main(int argc, char **argv)
{
    int numero = *argv[1] - '0';
    
    if (argc < 2 || argc > 3)
        return EXIT_FAILURE;

    if (numero < 1)
        return EXIT_FAILURE;

    vector<int> padres = {};
    generarHijos(numero-1,padres);

    return EXIT_SUCCESS;
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