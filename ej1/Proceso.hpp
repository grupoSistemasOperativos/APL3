#ifndef PROCESO_HPP
#define PROCESO_HPP

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <vector>

using namespace std;

class Proceso {
    public:
        Proceso(pid_t,vector<int>);
        pid_t getPid();
        void mostrar();
    private:
        pid_t pid;
        vector<pid_t> padres;
};



#endif