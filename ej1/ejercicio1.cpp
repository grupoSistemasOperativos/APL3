#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits>
#include <vector>
#include <string.h>

//void generarHijos(int, vector <int> );
int validarParametros(int, char *);
int mostrarAyuda(int ,char *);

using namespace std;
void generarHijos(int numero, vector<int> padres)
{
    if (numero == 0)
    {

        sleep(5);

        cout << "Proceso " << getpid() << " Pid: ";

        for (size_t i = 0; i < padres.size(); i++)
        {
            cout << padres[i] << ", ";
        }
        cout << endl;
        kill(getpid(), SIGTERM);
    }

    pid_t hijo1;
    hijo1 = fork();

    pid_t hijo2;

    if (hijo1 > 0)
    {
        hijo2 = fork();
    }
    else
    {
        padres.push_back((int)getppid());
        generarHijos(numero - 1, padres);
    }

    if (hijo2 == 0)
    {
        padres.push_back((int)getppid());
        generarHijos(numero - 1, padres);
    }

    sleep(10);
    waitpid(hijo1, NULL, 0);
    waitpid(hijo2, NULL, 0);
    cout << "Proceso " << getpid() << " Pid: ";

    for (size_t i = 0; i < padres.size(); i++)
    {
        cout << padres[i] << ", ";
    }

    cout << endl;

    kill(getpid(), SIGTERM);
}

int main(int argc, char **argv)
{
    if (mostrarAyuda(argc,argv[1]))
    exit(0);
    if (!validarParametros(argc, argv[1]))
    exit(0);

    

    int numero = atoi(argv[1]);
    vector<int>padres = {};
    generarHijos(numero - 1, padres);

    return EXIT_SUCCESS;
}

int mostrarAyuda(int cantPar,char *cad)
{

    if ((!strcmp(cad, "-h") || !strcmp(cad, "--help")) && cantPar == 2 )
    {
        cout << "HELP" << endl;
        cout << "NAME" << endl;
        cout << "    ejercicio1.exe - generar un arbol balanceado y completo de altura [NUMBER]" << endl;
        cout << "SYNOPSIS:" << endl;
        cout << "    ejercicio1.exe [NUMBER]  NUMBER > 0" << endl;
        cout << "DESCRIPTION:" << endl;
        cout << "    Muestra el arbol con altura [NUMBER], lista los pid de los hijos y sus ascendentes" << endl;
        return 1;
    }
    return 0;
}

int validarParametros(int cantParam, char *cad)
{

    int numero = atoi(cad);
    if (cantParam != 2 || numero < 1 )
    {
        cout << "Error de parametros" << endl;
        cout << "Para mostrar la ayuda : ejercicio1.exe [options]" << endl;
        cout << "options:" << endl;
        cout << "    -h" << endl;
        cout << "    --help" << endl;
        return 0;
    }


    return 1;
}
