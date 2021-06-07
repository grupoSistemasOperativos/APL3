#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <limits>
using namespace::std;

string inicializar(int cant);
int buscarApariciones(string& palabra, string& palabraOculta, char letra);
void reemplazar(string& palabraOculta,int pos);
bool esLetraIngresada(string& letras,char letraBuscada);
void limpiarPantalla();

int main() {
    string palabra = "probando";
    string letrasIngresadas = "";
    char letra;
    int intentos = 6;
    int aciertos = 0;
    int cantCambios;
    string palabraOculta = inicializar(palabra.size());
    system("clear");

    while(intentos > 0 && aciertos < palabra.size()) { 
        cout << palabraOculta << endl;
        
        cout << "ingrese una letra: ";
        cin >> letra;

        //validar solo que es letra y no otro caracter raro
        if(!esLetraIngresada(letrasIngresadas,letra)) {
            
            cantCambios = buscarApariciones(palabra,palabraOculta,letra);
            if(cantCambios > 0) {
                aciertos += cantCambios;
            }
            else {
                intentos--;
                cout << "la letra " << "\'" << letra << "\'" << " no se encuentra!\n"
                        "te quedan " << intentos << " vidas" << endl;

                limpiarPantalla();
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
        cout << "perdiste!" << endl;
    }
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

void limpiarPantalla() {

    cout << "Presione enter para continuar... " << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("clear");
}