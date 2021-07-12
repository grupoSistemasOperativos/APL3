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
#include <thread>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <vector>
#include <signal.h>

using namespace std;

int mostrarAyuda(int cantPar,char *cad);
int validarParametros(int cantParam, char *cad);
int buscarApariciones(string& palabra,string& palabraOculta,char letra);
bool esLetraIngresada(string& letras,char letraBuscada);
string obtenerPalabraDeArchivo();
string inicializar(int cant);
void hilo(int socketId);



int main(int argc, char **argv)
{

    if (mostrarAyuda(argc,argv[1]))
        exit(1);
    if (!(validarParametros(argc, argv[1])))
        exit(1);

    struct sockaddr_in serverConfig;
    memset(&serverConfig, '0', sizeof(serverConfig));

    signal(SIGINT,SIG_IGN);
    serverConfig.sin_family = AF_INET; // 127.0.0.1
    serverConfig.sin_addr.s_addr = htonl(INADDR_ANY);
    serverConfig.sin_port = htons(5000); // Mayor 1023

    int socketEscucha = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketEscucha, (struct sockaddr *)&serverConfig, sizeof(serverConfig));

    listen(socketEscucha, 10);

    while (true)
    {
        int socketComunicacion = accept(socketEscucha, (struct sockaddr *)NULL, NULL);
        
       // thread th(hilo, socketComunicacion);
       // th.detach();
        hilo(socketComunicacion);

        sleep(1);
    }

    return EXIT_SUCCESS;
}

int mostrarAyuda(int cantPar,char *cad)
{

    if (cantPar == 2  && (!strcmp(cad, "-h") || !strcmp(cad, "--help")))
    {
        cout << "HELP" << endl;
        cout << "NAME" << endl;
        cout << "    Servidor juego de ahorcado" << endl;
        cout << "SYNOPSIS:" << endl;
        cout << "    server.exe" << endl;
        cout << "DESCRIPTION:" << endl;
        cout << "    Permite conexiones por socket para el juego del ahorcado" << endl;
        return 1;
    }
    return 0;
}

int validarParametros(int cantParam, char *cad)
{

    if (cantParam > 1 )
    {
        cout << "Error de parametros" << endl;
        cout << "Para mostrar la ayuda : server.exe [options]" << endl;
        cout << "options:" << endl;
        cout << "    -h" << endl;
        cout << "    --help" << endl;
        return 0;
    }

    return 1;
}

int buscarApariciones(string& palabra,string& palabraOculta,char letra) {
    
    int cantIntercambios = 0;

    for (int i = 0; i < palabra.size(); i++)
    {
        if(palabra[i] == letra) {
            // if(palabraOculta[i] == letra)
            //     return -1;
            palabraOculta[i] = letra;
            cantIntercambios++;
        }
    }
    
    return cantIntercambios;
}

bool esLetraIngresada(string& letras,char letraBuscada) {
    
    if(letras.find(letraBuscada) < letras.size()){
        return true;
    }

    letras += letraBuscada;
    return false;
}


string obtenerPalabraDeArchivo() {

    fstream palabrasFile;
    srand(time(nullptr));
    palabrasFile.open("palabras.txt",ios::in);

    if(!palabrasFile) {
        cerr << "No se pudo abrir archivo" << endl;
        exit(1);
    }

    std::vector<string> palabras;
    string cadena;
    while(palabrasFile >> cadena) {
        palabras.push_back(cadena);
    }
    palabrasFile.close();
    
    return palabras[rand()%palabras.size()];
}

string inicializar(int cant) {
    string palabraOculta = "";
    
    while(cant-- > 0) {
        palabraOculta += "_";
    }

    return palabraOculta;
}


void hilo(int socketId)
{
    // Declaracion de variables, obtencion de palabra e inicializaciones.
    string palabra = obtenerPalabraDeArchivo();
    cout << palabra << endl;
    string letrasIngresadas = "";
    char letra;
    int intentos = 6;
    int aciertos = 0;
    int cantCambios;
    string palabraOculta = inicializar(palabra.size());
    char sendBuff[2000];
    char receiveBuff[2000];

    // Mientras que no se haya terminado el juego:
    while(intentos > 0 && aciertos < palabra.size()) { 
        //// Pasar palabra oculta al cliente
        snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", palabraOculta.c_str());
        write(socketId, sendBuff, strlen(sendBuff));

        cout << "Letras ingresadas: " << letrasIngresadas << endl;
        cout << "Le quedan " << intentos << " intentos" << endl;

        //// Recibir letra del cliente 
        read(socketId, receiveBuff, sizeof(receiveBuff) - 1);
        letra = receiveBuff[0];


        //// Comprobacion de letra ingresada
        letra = tolower(letra);
        if(!esLetraIngresada(letrasIngresadas,letra)) {
            
            cantCambios = buscarApariciones(palabra,palabraOculta,letra);
            if(cantCambios > 0) {
                aciertos += cantCambios;
                snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", "Correcto!");
            }
            else {
                intentos--;
                snprintf(sendBuff, sizeof(sendBuff), "%s '%c' %s \n%s%d%s \r\n", "La letra", letra,"no se encuentra!","Te quedan ",intentos," intentos!");
            }
        }
        else {
            snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", "Ya ingreso esa letra!");
        }
        write(socketId, sendBuff, strlen(sendBuff));
        sendBuff[0] = 0;
       // sleep(1);
        read(socketId, receiveBuff, sizeof(receiveBuff) - 1);
        //// Limpio ambos buffers para que no haya errores.
        memset(receiveBuff, 0, 2000);
        memset(sendBuff, 0, 2000);
    }

    
    write(socketId, "1", 2);
    cout << endl;
    // Comprobacion de juego terminado y avisar a cliente
    if(aciertos == palabra.size()) {
        snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", "GANASTE!");
    }
    else {
        snprintf(sendBuff, sizeof(sendBuff), "%s%s%s\r\n %s", "PERDISTE! la palabra era '", palabra.c_str(),"'","La proxima sera!");
    }
    

    write(socketId, sendBuff, strlen(sendBuff));
    read(socketId, receiveBuff, sizeof(receiveBuff) - 1);
    close(socketId);

}
