#include "menu.h"

///desplega un menu
char menu(char *msj,char *opc){
    char valor;
    int priVez=1;

    do{
        if(!priVez)
           printf("ERROR INGRESE VALORES VALIDOS \n");
        printf("%s",msj);
        fflush(stdin);
        scanf("%c",&valor);
        priVez=0;
    }while(!strchr(opc,TO_UPPER(valor)));

    return valor;
}
