#include "bibliotecas.h"
#include "SharedMemory.cpp"
using namespace::std;

string inicializar(int cant);
int buscarApariciones(const char *palabra,char *palabraOculta,char letra);
void reemplazar(string& palabraOculta,int pos);
bool esLetraIngresada(char* letras,char letraBuscada);
void signalHandler(int sig);
string obtenerPalabraDeArchivo();
bool clienteEnEjecucion();

string palabra = obtenerPalabraDeArchivo();
SharedMemory sharedMemory(palabra,inicializar(palabra.size()));


int main() {
    int cantCambios;
    string letra;

    sharedMemory.setPidServidor(getpid());
    
    sem_t *juego = sem_open("empezarJuego", O_CREAT, 0600, 0);
    sem_t *esperarLetra = sem_open("esperarLetra", O_CREAT, 0600, 0);
    sem_t *buscarLetra = sem_open("busquedaLetra", O_CREAT, 0600, 0);
    sem_t *determinarResultado = sem_open("determinarResultado", O_CREAT, 0600, 0);
    sem_t *calculoFin = sem_open("calculoFin", O_CREAT, 0600, 0);
    
    struct sigaction action;
    
    action.sa_handler = signalHandler;

    //sigaction(SIGINT, &action, NULL);
    sigaction(SIGUSR1, &action, NULL);

    datos* datosJuego = sharedMemory.getDatos();

    while(true) {
        cout << "proceso con pid: " << sharedMemory.getPidServidor() << endl;
        cout << "esperando cliente..." << endl;
        sem_wait(juego);
        cout << "Iniciando juego con palabra: " << palabra << endl;

        while(datosJuego->intentos > 0 && datosJuego->aciertos < palabra.size()) {
            
            cout << "Letras ingresadas: " << datosJuego->letrasIngresadas << endl;
            
            sem_post(calculoFin);
            sem_wait(esperarLetra);

            if(!esLetraIngresada(datosJuego->letrasIngresadas,datosJuego->letra)) {
                cantCambios = buscarApariciones(datosJuego->palabra,datosJuego->palabraOculta,datosJuego->letra);

                if(cantCambios > 0) {
                    datosJuego->aciertos += cantCambios;
                }
                else {
                    datosJuego->intentos--;
                }
                datosJuego->busquedaLetra = cantCambios;
            }
            else {
                    datosJuego->busquedaLetra = -1;
            }

            sem_post(buscarLetra);
        }
        
        if(datosJuego->aciertos == palabra.size()) {
            datosJuego->fin = 1;
        }
        else {
            datosJuego->fin = -1;
        }
        datosJuego->fin = 0;
        system("clear");
    }

    return 0;
}

string obtenerPalabraDeArchivo() {

    fstream palabrasFile;
    srand(time(nullptr));
    palabrasFile.open("archivosTexto/palabras.txt",ios::in);

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

int buscarApariciones(const char *palabra,char *palabraOculta,char letra) {
    
    int cantIntercambios = 0;

    while(*palabraOculta) {
        //cout << *palabra << endl;
        if(*palabra == letra) {
            *palabraOculta = letra;
            //cout << *palabraOculta << endl; 
            cantIntercambios++;
        }
        palabra++;
        palabraOculta++;
    }
    
    return cantIntercambios;
}

bool esLetraIngresada(char* letras,char letraBuscada) {

    while(*letras) {
        if(*letras == letraBuscada)
            return true;
        letras++;
    }

    *letras = letraBuscada;
    *(letras+1) = '\0';

    return false;
}

void signalHandler(int sig) {

    switch (sig)
    {
    case SIGINT:
            cout << "senial ignorada";
        break;
    case SIGUSR1:
            if(clienteEnEjecucion()) {
                cout << "No se puede finalizar el servidor si hay un cliente ejecutandose" << endl;
            }
            else {
                cout << "Cerrando servidor..." << endl;
                kill(getpid(),SIGTERM);
            }
        break;
    }

}

bool clienteEnEjecucion() {
    return sharedMemory.getPidCliente() > 0;
}