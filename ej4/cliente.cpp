#include "bibliotecas.hpp"
#include "memoria.cpp"

#define NO_ENCONTRADA 0
#define YA_INGRESADA -1
#define GANA 1

bool validar(const string& letra);
bool esLetra(char letra);
void limpiarPantalla();
void signalHandler(int sig);
bool validarEjecucionServidor();
bool seEjecutaCliente();


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

    sem_t *juego = sem_open("empezarJuego", O_CREAT, 0600, 0);
    sem_t *esperarLetra = sem_open("esperarLetra", O_CREAT, 0600, 0);
    sem_t *buscarLetra = sem_open("busquedaLetra", O_CREAT, 0600, 0);
    sem_t *verificarFin = sem_open("verificarFin", O_CREAT, 0600, 0);
    sem_t *esperarCliente = sem_open("cliente", O_CREAT, 0600, 0);
    sem_t *esperarServidor = sem_open("servidor",O_CREAT,0600,0);
    //sem_init(juego,1,0);
    
    struct sigaction action;
    action.sa_handler = signalHandler;

    sigaction(SIGINT, &action, NULL);   

    datosCompartidos *datosJuego = obtenerDatosCompartidos();
    datosJuego->procesos.pidCliente = getpid();
    // int val;
    sem_post(juego);
    // sem_getvalue(esperarServidor,&val);
    // cout << val << endl;
    sem_wait(esperarServidor);

    //while(!datosJuego->textos.fin) {
    while(datosJuego->textos.intentos > 0 && datosJuego->textos.aciertos < strlen(datosJuego->textos.palabra)) {
        
        //cout << datosJuego->palabra << endl;
        cout << "Intentos restantes: " << datosJuego->textos.intentos << endl
             << datosJuego->textos.palabraOculta << endl;
        do
        {
            cout << "ingrese una letra: ";
            cin >> letra;
        } while (!validar(letra));

        datosJuego->textos.letra = tolower(letra[0]);
        sem_post(esperarLetra);
        cout << "esperando validacion... " << endl;
        sem_wait(buscarLetra);

        if(datosJuego->textos.busquedaLetra == NO_ENCONTRADA) {

            cout << "la letra " << "\'" << datosJuego->textos.letra << "\'" << " no se encuentra!\n"
                    "te quedan " << datosJuego->textos.intentos << " vidas" << endl;
            limpiarPantalla();
        }
        else
            if(datosJuego->textos.busquedaLetra == YA_INGRESADA){
                cout << "ya ingreso esa letra!" << endl;
                limpiarPantalla();
            }
            else {
                system("clear");
            }
        //sem_wait(verificarFin);
    }
    sem_wait(verificarFin);

    if(datosJuego->textos.fin == GANA) {
        cout << "Ganaste!" << endl;
    }
    else {
        cout << "Perdiste!" << endl << "La palabra era " << "\'" << datosJuego->textos.palabra << "\'" << endl;
    }

    sem_post(esperarCliente);

    //liberarRecursos();
    datosJuego->procesos.pidCliente = -1;
    liberarMemoriaCompartida();
    return 0;
}


bool validar(const string& letra) {
    
    if(esLetra(letra.front()) && letra.size() == 1)
        return true;

    cout << "Ingrese una sola letra y que sea valida!" << endl;
    limpiarPantalla();
    
    return false;
}

bool esLetra(char letra) {
    return (letra >= 65 && letra <= 90) || (letra >= 97 && letra <= 122);
}

void limpiarPantalla() {

    do 
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Presione enter para continuar... " << endl;
    } while (cin.get() != '\n');

    system("clear");
}

void signalHandler(int sig) {

    sem_t *finalizarPartida = sem_open("finalizacion",O_CREAT,0600,0);

    datosCompartidos* datos = obtenerDatosCompartidos();

    datos->procesos.pidCliente = -1;
    
    // liberarRecursos();

    kill(datos->procesos.pidServidor,SIGUSR2);
    
    liberarMemoriaCompartida();
    
    sem_post(finalizarPartida);

    kill(getpid(),SIGTERM);
}

// void liberarSemaforos(sem_t *s1,sem_t *s2,sem_t* s3,sem_t* s4) {
//     sem_destroy(esperarLetra);
//     sem_destroy(determinarResultado);
//     sem_destroy(buscarLetra);
//     sem_destroy(verificarFin);
// }

bool validarEjecucionServidor() {
 
    return shm_open(memoriaCompartida, O_RDONLY, 0600) >= 0;
}

bool seEjecutaCliente() {
    return obtenerDatosCompartidos()->procesos.pidCliente >= 0;
}