#include "bibliotecas.hpp"

void liberarSemaforos();
void signalHandler(int sig);

void empezarJuego();

struct sigaction action;

int main() {

    empezarJuego();

    liberarSemaforos();

    return 0;
}

void empezarJuego() {


    int cantCambios;
    string palabraOculta;
    char letra;
    int aciertos;
    string letrasIngresadas; 
    
    while(true)
    {
        
        aciertos = 0;
        letrasIngresadas = "";
        
        sigemptyset(&action.sa_mask);
        action.sa_handler = signalHandler;
        
        action.sa_flags = SA_NODEFER;
        
        sigaction(SIGUSR1, &action, NULL);
        sigaction(SIGUSR2, &action,NULL);

        signal(SIGINT,SIG_IGN);

        sem_t *cliente = sem_open("cliente", O_CREAT, 0600, 0);
        sem_t *servidor = sem_open("servidor",O_CREAT,0600,0);
        
        if(!inicializarMemoria()) {
            cerr << "Solo una instancia de servidor se puede ejecutar a la vez" << endl;
            exit(1);
        }

        memoria* datosJuego = obtenerDatosCompartidos();

        string palabra = obtenerPalabraDeArchivo();
        if(palabra == "") {
            cerr << "Archivo vacio, no es posible continuar con la ejecucion." << endl;
            datosJuego->procesos.pidCliente = -1;
            kill(datosJuego->procesos.pidServidor,SIGUSR1);
        }

        inicializarDatos(datosJuego,palabra.size());

        cout << "Proceso con pid: " << datosJuego->procesos.pidServidor << endl;
        cout << "esperando cliente..." << endl;

        sem_wait(cliente);
        sem_post(servidor);

        system("clear");

        cout << "Iniciando juego con palabra: " << palabra << endl;

        while(!datosJuego->fin) {
            
            cout << "Letras ingresadas: " << letrasIngresadas << endl;

            while(sem_wait(cliente) < 0);

            if(!esLetraIngresada(letrasIngresadas,datosJuego->letraIngresada)) {
                cantCambios = buscarApariciones(palabra,datosJuego->palabraOculta,datosJuego->letraIngresada);

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

            sem_post(servidor);
            
            if(datosJuego->intentos == 0 || aciertos == palabra.size())
                datosJuego->fin = 1;

            sem_post(servidor);  
        }

        if(aciertos == palabra.size()) {
            datosJuego->fin = 1;
        }
        else {
            datosJuego->fin = -1;
            strcpy(datosJuego->palabraOculta,palabra.c_str());
        }

        sem_post(servidor);
        sem_wait(cliente);

        liberarSemaforos();
        liberarMemoriaCompartida();
        shm_unlink(memoriaCompartida);

        system("clear");
    }
}

void signalHandler(int sig) {

    switch (sig)
    {
    case SIGUSR1:
            if(seEjecutaCliente()) {
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

void liberarSemaforos() {

    sem_unlink("empezarJuego");
    sem_unlink("esperarLetra");
    sem_unlink("busquedaLetra");
    sem_unlink("verificarFin");
    sem_unlink("cliente");
    sem_unlink("servidor");
}