#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#define TO_UPPER(letra) ((letra)>=97 && (letra)<=122 ? (letra)-=32 : letra)

#include <string.h>
#include <stdio.h>

using namespace std;

char menuChar(string msj, string opc);
int menuAnio(string msj);
char* menuMes(string msj, string opc);
char * strlwr(char * s);
bool esMesValido(char * mes);

#define MENU_CLIENTE {"Ingrese una de las siguientes opciones: \n1. Para obtener la facturacion mensual. \n2. Para obtener la facturacion anual. \n3. Para obtener la facturacion anual media. \n4. Para salir."}
#define MESES "|enero|febrero|marzo|abril|mayo|junio|julio|agosto|septiembre|octubre|noviembre|diciembre|"

#endif // MENU_H_INCLUDED
