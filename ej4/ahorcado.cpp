#include "bibliotecas.hpp"
#include "SharedMemory.cpp"
using namespace::std;

string inicializar(int cant);
int buscarApariciones(string& palabra, string& palabraOculta, char letra);
void reemplazar(string& palabraOculta,int pos);
bool esLetraIngresada(string& letras,char letraBuscada);
void limpiarPantalla();
bool esLetra(char letra);
bool validar(const string& letra);
string obtenerPalabraDeArchivo();

int main() {
    int cantCambios;
    
    string palabra = obtenerPalabraDeArchivo();
    SharedMemory sharedMemory(palabra,inicializar(palabra.size()));

    //system("clear");
    datos* datosJuego = sharedMemory.getDatos();


    while(datosJuego->intentos > 0 && datosJuego->aciertos < palabra.size()) { 
        cout << datosJuego->palabraOculta << endl;
        do
        {
            cout << "ingrese una letra: ";
            cin >> datosJuego->letra;
        } while (!validar(datosJuego->letra));

        datosJuego->letra = tolower(datosJuego->letra[0]);

        if(!esLetraIngresada(datosJuego->letrasIngresadas,datosJuego->letra.front())) {
            
            cantCambios = buscarApariciones(datosJuego->palabra,datosJuego->palabraOculta,datosJuego->letra.front());
            if(cantCambios > 0) {
                datosJuego->aciertos += cantCambios;
            }
            else {
                datosJuego->intentos--;
                if(datosJuego->intentos) {
                    cout << "la letra " << "\'" << datosJuego->letra << "\'" << " no se encuentra!\n"
                            "te quedan " << datosJuego->intentos << " vidas" << endl;
                    limpiarPantalla();
                }//el else puede que sea innecesario
                else {
                    system("clear");
                }
            }
        }
        else {
                cout << "ya ingreso esa letra!" << endl;

                limpiarPantalla();
            }
    }

    if(datosJuego->aciertos == palabra.size()) {
        cout << "ganaste!" << endl;
    }
    else {
        cout << "perdiste!" << endl << "La palabra era " << "\'" << datosJuego->palabra << "\'" << endl;
    }
}

string obtenerPalabraDeArchivo() {

    fstream palabrasFile;
    srand(time(nullptr));
    palabrasFile.open("palabras.txt",ios::in);

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

int buscarApariciones(string& palabra,string& palabraOculta,char letra) {
    
    int cantIntercambios = 0;

    for (int i = 0; i < palabra.size(); i++)
    {
        if(palabra[i] == letra) {
            // if(palabraOculta[i] == letra)
            //     return -1;
            palabraOculta[i] = letra;
            cantIntercambios++;
        }
    }
    
    return cantIntercambios;
}

bool esLetraIngresada(string& letras,char letraBuscada) {
    
    if(letras.find(letraBuscada) < letras.size()){
        return true;
    }

    letras += letraBuscada;
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

bool validar(const string& letra) {
    
    if(esLetra(letra.front()) && letra.size() == 1)
        return true;

    cout << "Ingrese una sola letra y que sea valida!" << endl;
    limpiarPantalla();
    
    return false;
}