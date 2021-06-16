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

using namespace std;

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

void limpiarPantalla() {

    
    do 
    {
        // cin.clear();
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Presione enter para continuar... " << endl;
    } while (cin.get() != '\n');

    system("clear");
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
        printf("%s\n", sendBuff);
        write(socketId, sendBuff, strlen(sendBuff));

        read(socketId, receiveBuff, sizeof(receiveBuff) - 1);
        //printf("%s\n", receiveBuff);
        letra = receiveBuff[0];

        //// Recibir letra del cliente 
        //// Si letra no pertenece a palabra oculta avisarle al cliente

        // Comprobacion de juego terminado y avisar a cliente
        letra = tolower(letra);
        if(!esLetraIngresada(letrasIngresadas,letra)) {
            
            cantCambios = buscarApariciones(palabra,palabraOculta,letra);
            if(cantCambios > 0) {
                aciertos += cantCambios;
                // Avisar que la letra esta correcta.
                snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", "Correcto!");
            }
            else {
                intentos--;
                snprintf(sendBuff, sizeof(sendBuff), "%s '%c' %s\r\n", "La letra", letra,"no se encuentra!");
            }
        }
        else {
            snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", "Ya ingreso esa letra!");
        }
        write(socketId, sendBuff, strlen(sendBuff));
        sendBuff[0] = 0;
    }

    write(socketId, "", 0);

    if(aciertos == palabra.size()) {
        snprintf(sendBuff, sizeof(sendBuff), "%s\r\n", "GANASTE!");
    }
    else {
        snprintf(sendBuff, sizeof(sendBuff), "%s %s\r\n", "PERDISTE! la palabra era", palabra.c_str());
        //cout << "perdiste!" << endl << "La palabra era " << "\'" << palabra << "\'" << endl;
    }
    
    write(socketId, sendBuff, strlen(sendBuff));
    close(socketId);

}

int main()
{
    struct sockaddr_in serverConfig;
    memset(&serverConfig, '0', sizeof(serverConfig));

    serverConfig.sin_family = AF_INET; // 127.0.0.1
    serverConfig.sin_addr.s_addr = htonl(INADDR_ANY);
    serverConfig.sin_port = htons(5000); // Mayor 1023

    int socketEscucha = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketEscucha, (struct sockaddr *)&serverConfig, sizeof(serverConfig));

    listen(socketEscucha, 10);

    while (true)
    {
        int socketComunicacion = accept(socketEscucha, (struct sockaddr *)NULL, NULL);
        
        thread th(hilo, socketComunicacion);
        th.detach();

        // time_t ticks = time(NULL);
        // char sendBuff[2000];
        // snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));

        // write(socketComunicacion, sendBuff, strlen(sendBuff));
        // close(socketComunicacion);
        
        sleep(1);
    }

    return EXIT_SUCCESS;
}
