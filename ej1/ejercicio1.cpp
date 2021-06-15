#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <limits>
#include <vector>
#include <string.h>

using namespace std;

int validarParametros(int, char *);
int mostrarAyuda(int ,char *);
void generarHijos(int, vector<int>);

int main(int argc, char **argv)
{
    int numero;
    if(argc == 1)
    {
        cerr << "Error, debe pasar al menos un parametro" << endl;
        exit(1);
    }
    if (mostrarAyuda(argc,argv[1]))
        exit(1);
    if (!(numero = validarParametros(argc, argv[1])))
        exit(1);

    vector<int>padres = {};
    generarHijos(numero - 1, padres);

    return EXIT_SUCCESS;
}

void generarHijos(int numero, vector<int> padres)
{
    if (numero == 0)
    {
        sleep(10);
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

int mostrarAyuda(int cantPar,char *cad)
{

    if ((!strcmp(cad, "-h") || !strcmp(cad, "--help")) && cantPar == 2 )
    {
        cout << "HELP" << endl;
        cout << "NAME" << endl;
        cout << "    ejercicio1.exe - generar un arbol balanceado y completo de altura [NUMBER]" << endl;
        cout << "SYNOPSIS:" << endl;
        cout << "    ejercicio1.exe [NUMBER]  NUMBER > 1" << endl;
        cout << "DESCRIPTION:" << endl;
        cout << "    Muestra el arbol con altura [NUMBER], lista los pid de los hijos y sus ascendentes" << endl;
        return 1;
    }
    return 0;
}

int validarParametros(int cantParam, char *cad)
{

    int numero = atoi(cad);
    if (cantParam != 2 || numero <= 1 )
    {
        cout << "Error de parametros" << endl;
        cout << "Para mostrar la ayuda : ejercicio1.exe [options]" << endl;
        cout << "options:" << endl;
        cout << "    -h" << endl;
        cout << "    --help" << endl;
        return 0;
    }


    return numero;
}
