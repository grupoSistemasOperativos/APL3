#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <filesystem>
#include <system_error>

using namespace std;
namespace fs = std::experimental::filesystem;

typedef struct {
    char opcion;
    int anio;
    char mes[15];
} datoCliente;

struct stat info;

float obtenerFacturacionArchivo(string rutaArchivo);
float obtenerFacturacionAnio(int anio, string directorio, bool media);

int mostrarAyuda();

int main(int argc, char **argv) { 
    if (argc != 2) {
        cout << "Cantidad de parametros incorrecta. Uso: ./procesoServidor.exe -h/directorio" << endl;
        return EXIT_FAILURE;
    }

    string ruta = argv[1];

    if(ruta == "-h" || ruta == "--help") {
        mostrarAyuda();
        return EXIT_SUCCESS;
    }

    if(stat( ruta.c_str(), &info ) != 0 || !(info.st_mode & S_IFDIR)) {
        cout << ruta << " no es una ruta valida a un directorio" << endl;
        return EXIT_FAILURE;
    }

    datoCliente dato;
    dato.opcion = 1;

    do {
        mkfifo("fifoCliente", 0666);
        int fifo = open("fifoCliente", O_RDONLY);
        read(fifo, &dato, sizeof(dato));
        close(fifo);

        float totalFacturado;
        string mes(dato.mes);
        string rutaMes = ruta + "/" + to_string(dato.anio) + "/" + mes + ".txt";
        
        try
        {
            switch(dato.opcion) {
                case '1':
                    totalFacturado = obtenerFacturacionArchivo(rutaMes);
                    break;
                case '2':
                    totalFacturado = obtenerFacturacionAnio(dato.anio, ruta, false);
                    break;
                case '3':
                    totalFacturado = obtenerFacturacionAnio(dato.anio, ruta, true);
                    break;
                case '4':
                    unlink("fifoServidor");
                    return EXIT_SUCCESS;
            }
        }
        catch(const exception& e)
        {
            cerr << e.what() << endl;
            totalFacturado = -1;
        }
        

        if(dato.opcion != '4') {
            mkfifo("fifoServidor", 0666);
            int fifoServidor = open("fifoServidor", O_WRONLY);
            write(fifoServidor, &totalFacturado, sizeof(totalFacturado));
            close(fifoServidor);
        }
    } while (dato.opcion != '4');

    unlink("fifoServidor");
    return EXIT_SUCCESS;
}

int mostrarAyuda()
{
        cout << "HELP" << endl;
        cout << "NAME" << endl;
        cout << "    procesoServidor.exe - se encarga de procesar informacion contable." << endl;
        cout << "SYNOPSIS:" << endl;
        cout << "    procesoServidor.exe [DIRECTORIO]" << endl;
        cout << "DESCRIPTION:" << endl;
        cout << "    Se ejecutara y esperara a un procesoCliente a que solicite informacion, una vez solicitada, procesara y devolvera lo pedido." << endl;
    return 0;
}

float obtenerFacturacionArchivo(string rutaArchivo) {
    fstream archivoFacturacion;
    archivoFacturacion.open(rutaArchivo,ios::in);

    if(!archivoFacturacion) {
        throw runtime_error("Error: El archivo " + rutaArchivo + " no existe");
    }

    string linea;
    float facturacion = 0;
    while (getline(archivoFacturacion, linea)) {
        facturacion += stof(linea);
    }

    archivoFacturacion.close();
    
    return facturacion;
}

float obtenerFacturacionAnio(int anio, string directorio, bool media) {
    int cantMesesFacturados = 0;
    float facturacionAnual = 0;

    directorio = directorio + "/" + to_string(anio);

    for (const auto & entry : fs::directory_iterator(directorio)) {
        float facturacionMes = obtenerFacturacionArchivo(entry.path());
        facturacionAnual += facturacionMes;
        cantMesesFacturados++;
        std::cout << entry.path() << ": " << facturacionMes << std::endl;
    }

    if(media)
        return facturacionAnual/cantMesesFacturados;
    else
        return facturacionAnual;
}
