#include "../headers/bibliotecas.hpp"

#define NO_ENCONTRADA 0
#define YA_INGRESADA -1
#define GANA 1

bool mostrarAyuda(const char *cad);
void signalHandler(int sig);

int main(int argc, char *argv[]) {

    if(argc > 2)
    {
        cerr << "Error: Cantidad de parametros invalida" << endl;
        exit(1);
    }
    if(argc == 2)
    {
        if(!mostrarAyuda(argv[1]))
            cerr << "Error: parametro invalidos" << endl;
        exit(1);
    }

    if(!validarEjecucionServidor()) {
        cerr << "El servidor no es esta ejecutando" << endl;
        return 1;
    }

    if(seEjecutaCliente()) {
        cerr << "Ya existe un cliente ejecutandose " << endl;
        return 1; 
    }

    string letra;

    sem_t *cliente = sem_open("cliente", O_CREAT, 0600, 0);
    sem_t *servidor = sem_open("servidor",O_CREAT,0600,0);
    
    struct sigaction action;
    action.sa_handler = signalHandler;

    sigaction(SIGINT, &action, NULL);   
    sigaction(SIGHUP, &action, NULL);
    sigaction(SIGUSR1, &action, NULL);
    
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

    datosJuego->procesos.pidCliente = -1;
    liberarMemoriaCompartida();

    sem_post(cliente);

    return 0;
}

bool mostrarAyuda(const char *cad)
{
    if (!strcmp(cad, "-h") || !strcmp(cad, "--help") )
    {
        cout << "NAME" << endl;
        cout << "    cliente - ejecutable para iniciar ahorcado" << endl;
        cout << "SYNOPSIS:" << endl;
        cout << "    cliente [--help -h]" << endl;
        cout << "DESCRIPTION:" << endl;
        cout << "    Inicie el ejecutable para poder jugar al ahorcado" << endl;
        cout << "Aclaracion: el servidor debe estar corriendo previamente" << endl;
        return true;
    }
    return false;
}

void signalHandler(int sig) {

    sem_t *finalizarPartida = sem_open("finalizacion",O_CREAT,0600,0);

    memoria* datos = obtenerDatosCompartidos();

    datos->procesos.pidCliente = -1;

    if(sig != SIGUSR1)
    {
        kill(datos->procesos.pidServidor,SIGUSR2);
    }
    else 
    {
        cout << endl    << "El servidor finalizó inesperadamente." << endl
                        << "Finalizando cliente..." << endl;
    }
    
    liberarMemoriaCompartida();
    
    sem_post(finalizarPartida);

    kill(getpid(),SIGTERM);
}