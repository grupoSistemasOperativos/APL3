#include "../headers/utilitarios.hpp"

string obtenerPalabraDeArchivo() {

    fstream palabrasFile;
    srand(time(nullptr));
    palabrasFile.open("archivosTexto/palabras.txt",ios::in);

    if(!palabrasFile) {
        cerr << "No se pudo abrir archivo" << endl;
        exit(1);
    }

    std::vector<string> palabras;
    string cadena = "";
    while(palabrasFile >> cadena) {
        palabras.push_back(cadena);
    }

    if(cadena == "")
        return nullptr;

    palabrasFile.close();
    
    return palabras[rand()%palabras.size()];
}

int buscarApariciones(const string& palabra,char* palabraOculta,char letra) {
    
    int cantIntercambios = 0;

    for (int i = 0; i < palabra.size() ; i++)
    {
        if(palabra[i] == letra)
        {
            palabraOculta[i] = letra;
            cantIntercambios++;
        }

    }
    
    return cantIntercambios;
}

bool esLetraIngresada(string& letras,char letraBuscada) {

    for(int i = 0; i < letras.size(); i++)
    {
        if(letras[i] == letraBuscada)
            return true;
    }

    letras += letraBuscada;

    return false;
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
}