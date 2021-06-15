#include "bibliotecas.hpp"
#include "memoria.cpp"

using namespace::std;


int buscarApariciones(const string& palabra,char *palabraOculta,char letra);
bool esLetraIngresada(string& letras,char letraBuscada);
void reemplazar(string& palabraOculta,int pos);
bool esLetraIngresada(char* letras,char letraBuscada);
const string* obtenerPalabraDeArchivo();
bool clienteEnEjecucion();
void liberarSemaforos();
void restaurarSemaforos();
void signalHandler(int sig);
void empezarJuego();

struct sigaction action;

int main() {

    empezarJuego();

    liberarSemaforos();

    return 0;
}

void empezarJuego() {


    string letra;
    int cantCambios;
    // struct sigaction action;
    //sigignore(SIGINT);    
    
    while(true) {
        
        int aciertos = 0;
        //int letrasModificadas = 
        int fin;
        string letrasIngresadas = "";
        string palabraOculta;
        char letra;
        
        sigemptyset(&action.sa_mask);
        action.sa_handler = signalHandler;
        
        action.sa_flags = SA_NODEFER;
        
        sigaction(SIGUSR1, &action, NULL);
        sigaction(SIGUSR2, &action,NULL);

        signal(SIGINT,SIG_IGN);

        sem_t *juego = sem_open("empezarJuego", O_CREAT, 0600, 0);
        sem_t *esperarLetra = sem_open("esperarLetra", O_CREAT, 0600, 0);
        sem_t *buscarLetra = sem_open("busquedaLetra", O_CREAT, 0600, 0);
        sem_t *verificarFin = sem_open("verificarFin", O_CREAT, 0600, 0);
        sem_t *esperarCliente = sem_open("cliente", O_CREAT, 0600, 0);
        sem_t *esperarServidor = sem_open("servidor",O_CREAT,0600,0);
        
        if(!inicializarMemoria()) {
            cerr << "Solo una instancia de servidor se puede ejecutar a la vez" << endl;
            exit(1);
        }

        memoria* datosJuego = obtenerDatosCompartidos();

        const string *palabra = obtenerPalabraDeArchivo();
        if(!palabra) {
            cerr << "Archivo vacio, no es posible continuar con la ejecucion." << endl;
            datosJuego->procesos.pidCliente = -1;
            kill(datosJuego->procesos.pidServidor,SIGUSR1);
        }

        inicializarDatos(datosJuego,palabra->size());

        //delete(palabraArchivo);

        cout << "Proceso con pid: " << datosJuego->procesos.pidServidor << endl;
        cout << "esperando cliente..." << endl;

        sem_post(esperarServidor);
        // int val;
        // sem_getvalue(esperarServidor,&val);
        // cout << val << endl;
        sem_wait(juego);
        system("clear");
        cout << "Iniciando juego con palabra: " << *palabra << endl;

        while(!datosJuego->fin) {
            
            cout << "Letras ingresadas: " << letrasIngresadas << endl;

            while(sem_wait(esperarLetra) < 0);

            if(!esLetraIngresada(letrasIngresadas,datosJuego->letraIngresada)) {
                cantCambios = buscarApariciones(*palabra,datosJuego->palabraOculta,datosJuego->letraIngresada);

                if(cantCambios > 0) {
                    aciertos += cantCambios;
                }
                else {
                    cout << "pierde intento " << endl;
                    datosJuego->intentos--;
                }
            }
            else {
                    cantCambios = -1;
            }
            datosJuego->resultadoBusqueda = cantCambios; 

            sem_post(buscarLetra);
            
            if(datosJuego->intentos == 0 || aciertos == palabra->size())
                datosJuego->fin = 1;

            sem_post(verificarFin);  
        }

        if(aciertos == palabra->size()) {
            datosJuego->fin = 1;
        }
        else {
            datosJuego->fin = -1;
            strcpy(datosJuego->palabraOculta,palabra->c_str());
        }

        sem_post(verificarFin);
        sem_wait(esperarCliente);

        liberarSemaforos();
        liberarMemoriaCompartida();
        shm_unlink(memoriaCompartida);     

        //kill(datosJuego->procesos.pidServidor,SIGUSR2);

        system("clear");
    }
}

const string* obtenerPalabraDeArchivo() {

    fstream palabrasFile;
    srand(time(nullptr));
    palabrasFile.open("archivosTexto/palabras.txt",ios::in);

    if(!palabrasFile) {
        cerr << "No se pudo abrir archivo" << endl;
        exit(1);
    }

    std::vector<string> palabras;
    string *cadena = new string("");
    while(palabrasFile >> *cadena) {
        palabras.push_back(*cadena);
    }

    if(*cadena == "")
        return nullptr;

    palabrasFile.close();
    
    *cadena = palabras[rand()%palabras.size()];

    return (const string*)cadena;
}

int buscarApariciones(const string& palabra,char* palabraOculta,char letra) {
    
    int cantIntercambios = 0;


    for (int i = 0; i < palabra.size() ; i++)
    {
        if(palabra[i] == letra)
        {
            palabraOculta[i] = letra;
            cantIntercambios++;
        }

    }
    
    return cantIntercambios;
}

bool esLetraIngresada(string& letras,char letraBuscada) {

    for(int i = 0; i < letras.size(); i++)
    {
        if(letras[i] == letraBuscada)
            return true;
    }

    letras += letraBuscada;

    return false;
}

void signalHandler(int sig) {

    switch (sig)
    {
    case SIGUSR1:
            if(clienteEnEjecucion()) {
                cout << "No se puede finalizar el servidor si hay un cliente ejecutandose" << endl;
            }
            else {
                cout << "Cerrando servidor..." << endl;
                liberarMemoriaCompartida();
                liberarSemaforos();
                shm_unlink(memoriaCompartida);
                kill(getpid(),SIGTERM);
            }
        break;
    case SIGUSR2:
            cout << "finalizando partida..." << endl;
            sem_t *esperarFinalizacion = sem_open("finalizacion",O_CREAT,0600,0);

            sem_wait(esperarFinalizacion);

            liberarSemaforos();
            liberarMemoriaCompartida();
            shm_unlink(memoriaCompartida);

            system("clear");
            sem_unlink("finalizacion");
            empezarJuego();
        break;
    }
}

bool clienteEnEjecucion() {
    return obtenerDatosCompartidos()->procesos.pidCliente >= 0;
}

void restaurarSemaforos() {
    sem_t *juego = sem_open("empezarJuego", O_CREAT, 0600, 0);

    sem_unlink("empezarJuego");
}

void liberarSemaforos() {

    sem_unlink("empezarJuego");
    sem_unlink("esperarLetra");
    sem_unlink("busquedaLetra");
    sem_unlink("verificarFin");
    sem_unlink("cliente");
    sem_unlink("servidor");
}