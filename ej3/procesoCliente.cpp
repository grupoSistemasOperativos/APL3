#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits>
#include "menu.h"

using namespace std;

typedef struct {
    char opcion;
    int anio;
    char mes[15];
} datoCliente;

struct stat info;

int main(int argc, char **argv) {

    if (argc > 2) {
        cout << "Cantidad de parametros incorrecta. Uso: ./procesoCliente.exe -h" << endl;
        return EXIT_FAILURE;
    }

    if(argc == 2) {
        if(argv[1] == "-h" || argv[1] == "--help") {   
            cout << "Ejecute este programa sin ningun parametro y se le va a desplegar un menu con opciones para ejecutar. Para funcionar debe tambien estar ejecutado el procesoServidor" << endl;
            return EXIT_SUCCESS;
        } else {
            cout << "Cantidad de parametros incorrecta. Uso: ./procesoCliente.exe -h" << endl;            
            return EXIT_FAILURE;
        }
    }

    datoCliente dato;
    
    do {

        dato.opcion = menuChar(MENU_CLIENTE, "1234");
        switch(dato.opcion) {
            case '1':
                dato.anio = menuAnio("Ingrese Anio: ");
                strcpy(dato.mes, menuMes("Ingrese Mes: ", MESES));
                break;
            case '2':
                dato.anio = menuAnio("Ingrese Anio: ");
                break;
            case '3':
                dato.anio = menuAnio("Ingrese Anio: ");
                break;
            case '4':
                break;
            default:
                dato.opcion = '4';
                break;
        }
        mkfifo("fifoCliente", 0666);
        int fifoCliente = open("fifoCliente", O_WRONLY);
        write(fifoCliente, &dato, sizeof(dato));
        close(fifoCliente);

        if(dato.opcion != '4') {
            float totalFacturado;

            mkfifo("fifoServidor", 0666);
            int fifoServidor = open("fifoServidor", O_RDONLY);
            read(fifoServidor, &totalFacturado, sizeof(totalFacturado));
            close(fifoServidor);
            
            if(totalFacturado == -1)
                cout << "El anio y/o mes ingresado no posee facturacion." << endl;
            else {
                switch(dato.opcion) {
                    case '1':
                        cout << "El total facturado en el mes de " << dato.mes << " del año " << dato.anio << " es de: $" << totalFacturado << endl;
                        break;
                    case '2':
                        cout << "El total facturado en el año " << dato.anio << " es de: $" << totalFacturado << endl;
                        break;
                    case '3':
                        cout << "La media del total facturado del año " << dato.anio << " es de: $" << totalFacturado << endl;
                        break;
                }
            }
        }

        cin.clear();
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');

    } while (dato.opcion != '4');

    return EXIT_SUCCESS;
}

char menuChar(string msj,string opc){
    char valor;
    int priVez=1;

    do{
        if(!priVez)
           printf("ERROR INGRESE VALORES VALIDOS \n");
        cout << msj << endl;
        //fflush(stdin);
        scanf("%c",&valor);
        priVez=0;
        cin.ignore();
    }while(!strchr(opc.c_str(),TO_UPPER(valor)));

    return valor;
}

int menuAnio(string msj){
    int valor;
    int priVez=1;

    do{
        if(!priVez)
           printf("ERROR INGRESE VALORES VALIDOS \n");
        cout << msj << endl;
        scanf("%d",&valor);
        priVez=0;
        cin.ignore();
    }while(valor < 1900 || valor > 3000);

    return valor;
}

char* menuMes(string msj,string opc){
    char* valor;
    char* valorAyuda;
    int priVez=1;

    do{
        if(!priVez)
            cout << "ERROR INGRESE VALORES VALIDOS: " << opc << "\n" << endl;
        cout << msj << endl;
        scanf("%s",valor);
        priVez=0;
        valor = strlwr(valor);
        cin.ignore();
    } while (!esMesValido(valor));

    return valor;
}

bool esMesValido(char * mes) {
    cout << mes << endl;
    if(!strcmp(mes,"enero"))
        return true;
    else if(!strcmp(mes,"febrero"))
        return true;
    else if(!strcmp(mes,"marzo"))
        return true;
    else if(!strcmp(mes,"abril"))
        return true;
    else if(!strcmp(mes,"mayo"))
        return true;
    else if(!strcmp(mes,"junio"))
        return true;
    else if(!strcmp(mes,"julio"))
        return true;
    else if(!strcmp(mes,"agosto"))
        return true;
    else if(!strcmp(mes,"septiembre"))
        return true;
    else if(!strcmp(mes,"octubre"))
        return true;
    else if(!strcmp(mes,"noviembre"))
        return true;
    else if(!strcmp(mes,"diciembre"))
        return true;
    return false;
}

char * strlwr(char * s)
{
    char *t = s;
 
    if (!s) {
        return 0;
    }
 
    int i = 0;
    while ( *t != '\0' ) {
        if (*t >= 'A' && *t <= 'Z' ) {
            *t = *t + ('a' - 'A');
        }
        t++;
    }
    return s;
}
