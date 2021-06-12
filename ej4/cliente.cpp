#include "bibliotecas.h"

#define NO_ENCONTRADA 0
#define YA_INGRESADA -1
#define GANA 1

bool validar(const string& letra);
bool esLetra(char letra);
void limpiarPantalla();
//void signalHandler(int sig);

int main() {
    SharedMemory sm;
    string letra;
    sem_t *juego = sem_open("empezarJuego", O_CREAT, 0600, 0);
    sem_t *esperarLetra = sem_open("esperarLetra", O_CREAT, 0600, 0);
    sem_t *buscarLetra = sem_open("busquedaLetra", O_CREAT, 0600, 0);
    sem_t *determinarResultado = sem_open("determinarResultado", O_CREAT, 0600, 0);
    sem_t *calculoFin = sem_open("calculoFin", O_CREAT, 0600, -1);

    datos *datosJuego = sm.getDatos();

    sm.setPidCliente(getpid());

    sem_post(juego);

    while(!datosJuego->fin) {

        //cout << datosJuego->palabra << endl;
        cout << "Intentos restantes: " << datosJuego->intentos << endl
             << datosJuego->palabraOculta << endl;
        do
        {
            cout << "ingrese una letra: ";
            cin >> letra;
        } while (!validar(letra));

        datosJuego->letra = tolower(letra[0]);

        sem_post(esperarLetra);
        sem_wait(buscarLetra);

        if(datosJuego->busquedaLetra == NO_ENCONTRADA) {

            cout << "la letra " << "\'" << datosJuego->letra << "\'" << " no se encuentra!\n"
                    "te quedan " << datosJuego->intentos << " vidas" << endl;
            limpiarPantalla();
        }
        else
            if(datosJuego->busquedaLetra == YA_INGRESADA){
                cout << "ya ingreso esa letra!" << endl;
                limpiarPantalla();
            }
            else {
                system("clear");
            }

        sem_wait(calculoFin);
    }

    if(datosJuego->fin == GANA) {
        cout << "Ganaste!" << endl;
    }
    else {
        cout << "Perdiste!" << endl << "La palabra era " << "\'" << datosJuego->palabra << "\'" << endl;
    }

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

// void signalHandler(int sig) {
//     if(sig == SIGUSR1) 
//         kill()
// }