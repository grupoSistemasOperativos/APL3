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

int mostrarAyuda();

int main(int argc, char **argv) {

    if (argc > 2) {
        cout << "Cantidad de parametros incorrecta. Uso: ./procesoCliente.exe -h" << endl;
        return EXIT_FAILURE;
    }

    if(argc == 2) {
        if(!strcmp(argv[1],"-h") || !strcmp(argv[1],"--help")) {   
            mostrarAyuda();
            return EXIT_SUCCESS;
        } else {
            cout << "Parametro invalido" << endl;            
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
                        cout << "El total facturado en el mes de " << dato.mes << " del a??o " << dato.anio << " es de: $" << totalFacturado << endl;
                        break;
                    case '2':
                        cout << "El total facturado en el a??o " << dato.anio << " es de: $" << totalFacturado << endl;
                        break;
                    case '3':
                        cout << "La media del total facturado del a??o " << dato.anio << " es de: $" << totalFacturado << endl;
                        break;
                }
                cin.clear();
            }
            do {
                cout << "Presione enter para continuar..." << endl;
            }while(cin.get()!='\n');

            system("clear");
        }

        cin.clear();

    } while (dato.opcion != '4');

    unlink("fifoCliente");
    return EXIT_SUCCESS;
}

int mostrarAyuda()
{
        cout << "HELP" << endl;
        cout << "NAME" << endl;
        cout << "    procesoCliente.exe - brinda informacion contable." << endl;
        cout << "SYNOPSIS:" << endl;
        cout << "    procesoCliente.exe" << endl;
        cout << "DESCRIPTION:" << endl;
        cout << "    Se desplegara un menu donde podra solicitar distinto tipo de informacion sobre facturaciones respecto a mes y/o anio." << endl;
    return 0;
}

char menuChar(string msj,string opc){
    char valor;
    int priVez=1;

    do{
        if(!priVez)
           printf("ERROR INGRESE VALORES VALIDOS\n");
        cout << msj << endl;
        //fflush(stdin);
        cin >> valor;
        priVez=0;
        cin.clear();
        cin.ignore();
    }while(!strchr(opc.c_str(),TO_UPPER(valor)));

    return valor;
}

int menuAnio(string msj){
    int valor;
    int priVez=1;

    do{
        if(!priVez)
           printf("ERROR INGRESE VALORES VALIDOS PARA EL ANIO\n");
        cout << msj << endl;
        cin >> valor;
        priVez=0;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }while(valor < 1900 || valor > 3000);

    return valor;
}

char* menuMes(string msj,string opc){
    char* valor;
    char* valorAyuda;
    int priVez=1;

    do{
        if(!priVez)
            cout << "ERROR INGRESE VALORES VALIDOS: " << "\n" << endl;
        cout << msj << endl;
        cin >> valor;
        priVez=0;
        valor = strlwr(valor);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (!esMesValido(valor));

    return valor;
}

bool esMesValido(char * mes) {
    
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
