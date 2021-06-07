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
bool esLetra(char letra);

int main() {
    string palabra = "probando";
    string letrasIngresadas = "";
    char letra;
    int intentos = 6;
    int aciertos = 0;
    int cantCambios;
    string palabraOculta = inicializar(palabra.size());
    bool res;
    system("clear");

    while(intentos > 0 && aciertos < palabra.size()) { 
        cout << palabraOculta << endl;
        
        cout << "ingrese una letra: ";
        cin >> letra;

        //validar solo que es letra y no otro caracter raro
        if( (res = esLetra(letra)) && !esLetraIngresada(letrasIngresadas,letra)) {
            
            cantCambios = buscarApariciones(palabra,palabraOculta,letra);
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
        else
            if(!res) {
                cout << "ingrese una letra valida!" << endl;
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

    cout << "Presione enter para continuar... " << endl;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    system("clear");
}