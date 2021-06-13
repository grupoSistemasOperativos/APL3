#include "Proceso.hpp"

Proceso::Proceso(pid_t pid,vector<int> padres) {
    
    this->pid = pid;
    this->padres = padres;
}

void Proceso::mostrar() {
    cout << "Proceso " << this->pid << ": ";

    int i;
    for (i = 0; i < padres.size()-1; i++)
    {
        cout << "Pid " << padres[i] << ",";
    }
    cout << "Pid " << padres[i] << endl;
}

pid_t Proceso::getPid() {
    return this->pid;
}