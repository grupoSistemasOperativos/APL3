#include "Proceso.hpp"

Proceso::Proceso(pid_t pid,vector<int> padres) {
    
    this->pid = pid;
    this->padres = padres;
}

void Proceso::mostrar() {
    cout << "Proceso " << this->pid << " Pid ";
        for (size_t i = 0; i < padres.size(); i++)
    {
        cout << padres[i] << ",";
    }

    cout << endl;
}

pid_t Proceso::getPid() {
    return this->pid;
}