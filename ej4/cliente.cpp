#include "bibliotecas.hpp"

#define NO_ENCONTRADA 0
#define YA_INGRESADA -1
#define GANA 1

void signalHandler(int sig);

int main() {

    if(!validarEjecucionServidor()) {
        cerr << "El servidor no es esta ejecutando" << endl;
        return 1;
    }

    if(seEjecutaCliente()) {
        cerr << "Ya existe un cliente ejecutandose " << endl;
        return 1; 
    }

    string letra;

    sem_t *verificarFin = sem_open("verificarFin", O_CREAT, 0600, 0);
    sem_t *cliente = sem_open("cliente", O_CREAT, 0600, 0);
    sem_t *servidor = sem_open("servidor",O_CREAT,0600,0);
    
    struct sigaction action;
    action.sa_handler = signalHandler;

    sigaction(SIGINT, &action, NULL);   

    memoria *datosJuego = obtenerDatosCompartidos();
    datosJuego->procesos.pidCliente = getpid();

    sem_post(cliente);

    sem_wait(servidor);

    while(!datosJuego->fin) {
        
        cout << "Intentos restantes: " << datosJuego->intentos << endl
             << datosJuego->palabraOculta << endl;
        do
        {
            cout << "ingrese una letra: ";
            cin >> letra;
        } while (!validar(letra));

        datosJuego->letraIngresada = tolower(letra[0]);

        sem_post(cliente);

        cout << "esperando validacion... " << endl;

        sem_wait(servidor);

        if(datosJuego->resultadoBusqueda == NO_ENCONTRADA) {

            cout << "La letra " << "\'" << datosJuego->letraIngresada << "\'" << " no se encuentra!\n"
                    "Te quedan " << datosJuego->intentos << " vidas" << endl;
            limpiarPantalla();
        }
        else
            if(datosJuego->resultadoBusqueda == YA_INGRESADA){
                cout << "Ya ingreso esa letra!" << endl;
                limpiarPantalla();
            }
            else {
                system("clear");
            }

        sem_wait(servidor);
    }
    sem_wait(servidor);

    if(datosJuego->fin == GANA) {
        cout << "Ganaste!" << endl;
    }
    else {
        cout << "Perdiste!" << endl << "La palabra era " << "\'" << datosJuego->palabraOculta << "\'" << endl;
    }

    sem_post(cliente);

    datosJuego->procesos.pidCliente = -1;
    liberarMemoriaCompartida();

    return 0;
}

void signalHandler(int sig) {

    sem_t *finalizarPartida = sem_open("finalizacion",O_CREAT,0600,0);

    memoria* datos = obtenerDatosCompartidos();

    datos->procesos.pidCliente = -1;

    kill(datos->procesos.pidServidor,SIGUSR2);
    
    liberarMemoriaCompartida();
    
    sem_post(finalizarPartida);

    kill(getpid(),SIGTERM);
}