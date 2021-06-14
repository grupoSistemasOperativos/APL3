#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <experimental/filesystem>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
namespace fs = std::experimental::filesystem;

typedef struct {
    char opcion;
    int anio;
    char mes[15];
} datoCliente;

float obtenerFacturacionArchivo(string rutaArchivo);
float obtenerFacturacionAnio(int anio, string directorio, bool media);

int main(int argc, char **argv) { 
    if (argc != 2) {
        cout << "Cantidad de parametros incorrecta. Uso: ./procesoServidor.exe -h/directorio" << endl;
        return EXIT_FAILURE;
    }

    string ruta = argv[1];
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
                return EXIT_SUCCESS;
        }

        if(dato.opcion != '4') {
            mkfifo("fifoServidor", 0666);
            int fifoServidor = open("fifoServidor", O_WRONLY);
            write(fifoServidor, &totalFacturado, sizeof(totalFacturado));
            close(fifoServidor);
        }
    } while (dato.opcion != '4');

    return EXIT_SUCCESS;
}

float obtenerFacturacionArchivo(string rutaArchivo) {
    fstream archivoFacturacion;
    archivoFacturacion.open(rutaArchivo,ios::in);

    if(!archivoFacturacion) {
        cerr << "No se pudo abrir archivo en ruta: " << rutaArchivo << endl;
        exit(1);
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