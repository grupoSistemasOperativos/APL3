#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
#include <vector>
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
    string palabra = obtenerPalabraDeArchivo();
    cout << palabra << endl;
    string letrasIngresadas = "";
    string letra;
    int intentos = 6;
    int aciertos = 0;
    int cantCambios;
    string palabraOculta = inicializar(palabra.size());

    //system("clear");

    while(intentos > 0 && aciertos < palabra.size()) { 
        cout << palabraOculta << endl;
        do
        {
            cout << "ingrese una letra: ";
            cin >> letra;
        } while (!validar(letra));

        letra = tolower(letra[0]);

        if(!esLetraIngresada(letrasIngresadas,letra.front())) {
            
            cantCambios = buscarApariciones(palabra,palabraOculta,letra.front());
            if(cantCambios > 0) {
                aciertos += cantCambios;
            }
            else {
                intentos--;
                if(intentos) {
                    cout << "la letra " << "\'" << letra << "\'" << " no se encuentra!\n"
                            "te quedan " << intentos << " vidas" << endl;
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

    if(aciertos == palabra.size()) {
        cout << "ganaste!" << endl;
    }
    else {
        cout << "perdiste!" << endl << "La palabra era " << "\'" << palabra << "\'" << endl;
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