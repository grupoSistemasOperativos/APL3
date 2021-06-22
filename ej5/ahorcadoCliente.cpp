#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <vector>


using namespace std;

int mostrarAyuda(int cantPar,char *cad);
int validarParametros(int cantParam, char *cad);
bool esLetra(char letra);
bool validar(const string& letra);


int main(int argc, char *argv[])
{

    if (mostrarAyuda(argc,argv[1]))
        exit(1);
    if (!(validarParametros(argc, argv[1])))
        exit(1);

    struct sockaddr_in socketConfig;
    memset(&socketConfig, '0', sizeof(socketConfig));

    socketConfig.sin_family = AF_INET;
    socketConfig.sin_port = htons(5000);
    inet_pton(AF_INET, argv[1], &socketConfig.sin_addr);

    int socketComunicacion = socket(AF_INET, SOCK_STREAM, 0);

    int resultadoConexion = connect(socketComunicacion,
        (struct sockaddr *)&socketConfig, sizeof(socketConfig));

    if (resultadoConexion < 0)
    {
        cout << "Error en la conexión" << endl;
        return EXIT_FAILURE;
    }

    char bufferRead[2000] = "";
    char bufferWrite[2000] = "";
    int bytesRecibidos = 0;
    string letra;

    // Mientras el juego no haya finalizado:
    while ((bytesRecibidos = read(socketComunicacion, bufferRead, sizeof(bufferRead) - 1)) > 0 && 
        strcmp(bufferRead,"1") && strcmp(bufferRead,"0")){
        //// Recibir palabra oculta de servidor y mostrar
        bufferRead[bytesRecibidos] = 0;
        printf("%s \n", bufferRead);

        //// Ingresar una letra y validar que sea letra.
        do
        {
            cout << "ingrese una letra: ";
            cin >> letra;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            system("clear");
        } while (!validar(letra));
        //// Enviar letra a servidor
        snprintf(bufferWrite, sizeof(bufferWrite), "%s\r\n", letra.c_str());
        write(socketComunicacion, bufferWrite, strlen(bufferWrite));

        //// Limpio ambos buffers para que no haya errores.
        memset(bufferWrite, 0, 2000);
        memset(bufferRead, 0, 2000);

        //// Recibir respuesta si la letra fue incorrecta, en caso contrario repetir
        bytesRecibidos = read(socketComunicacion, bufferRead, sizeof(bufferRead) - 1);
        bufferRead[bytesRecibidos] = '\0';
        printf("%s\n", bufferRead);

        write(socketComunicacion, "000", 2);
    }
    read(socketComunicacion, bufferRead, sizeof(bufferRead) - 1);
    printf("%s\n", bufferRead);
    write(socketComunicacion, bufferWrite, strlen(bufferWrite));
    close(socketComunicacion);
    return EXIT_SUCCESS;
}


int mostrarAyuda(int cantPar,char *cad)
{

    if (cantPar == 2  && (!strcmp(cad, "-h") || !strcmp(cad, "--help")))
    {
        cout << "HELP" << endl;
        cout << "NAME" << endl;
        cout << "    Cliente juego de ahorcado" << endl;
        cout << "SYNOPSIS:" << endl;
        cout << "    cliente.exe [IP]" << endl;
        cout << "DESCRIPTION:" << endl;
        cout << "    Permite conectarse al servidor y jugar el juego del ahorcado" << endl;
        return 1;
    }
    return 0;
}

int validarParametros(int cantParam, char *cad)
{

    if (cantParam > 2 )
    {
        cout << "Error de parametros" << endl;
        cout << "Para mostrar la ayuda : cliente.exe [options]" << endl;
        cout << "options:" << endl;
        cout << "    -h" << endl;
        cout << "    --help" << endl;
        return 0;
    }

    return 1;
}


bool esLetra(char letra) {
    return (letra >= 65 && letra <= 90) || (letra >= 97 && letra <= 122);
}

bool validar(const string& letra) {
    
    if(esLetra(letra.front()) && letra.size() == 1)
        return true;

    cout << "Ingrese una sola letra y que sea valida!" << endl;
    //limpiarPantalla();
    
    return false;
}
