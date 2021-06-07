#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace::std;

string inicializar(int cant);
int buscarLetra(string& palabra,char letra);
void reemplazar(string& palabraOculta,int pos);

int main() {
    string palabra = "probando";

    char letra;
    int intentos = 6;
    int aciertos = 0;

    string palabraOculta = inicializar(palabra.size());

    while(intentos > 0 && aciertos < palabra.size()) { 
        cout << palabraOculta << endl;
        
        cout << "ingrese una letra: ";
        cin >> letra;

        int pos = buscarLetra(palabra,letra);
        //cout << pos << endl;
        
        if(pos < palabra.size()) {
            palabraOculta[pos] = palabra[pos];
            reemplazar(palabra,pos);
            aciertos++;
        }

        else
            intentos--;
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

int buscarLetra(string& palabra,char letra) {
    
    return palabra.find(letra);
}

void reemplazar(string& palabra,int pos) {
    
    if(pos < palabra.size())
        palabra.replace(pos,1,"|");
}