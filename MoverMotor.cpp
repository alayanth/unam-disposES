#include <stdio.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <sstream>
#include <stdlib.h>

//Dirección de salida de datos del puerto paralelo
#define PARALLEL 0x378
//revisar muy bien cuantos grados da el motor por cada paso
#define GRADOS_POR_PASO 6.0
//Retraso para el movimiento del motor el sweet spot esta entre 50 y 100
#define VALOR 80

using namespace std;

//funcion de salida al puerto paralelo o serie
typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);

void moverMotor(int grados,int dir);
HINSTANCE cargaLibreria(void);

int main(int argc, char **argv) {
    int grados;
    int sentido;
    if(argc > 2){
        grados = strtol(argv[1],NULL,10);
        sentido = strtol(argv[2],NULL,10);
    }
    else{
        grados = 360;
        sentido = 0;
    }
    moverMotor(grados,sentido);
    return 0;
}

void moverMotor(int grados, int dir){
    HINSTANCE hLib = cargaLibreria();
    oupfuncPtr out;
    out = (oupfuncPtr) GetProcAddress(hLib, "Out32");
    if (out == NULL) {
        cout<<"GetProcAddress for Oup32 Failed.\n";
        cin.get();
        exit(EXIT_FAILURE);
    }
   float pasos=0;
   int i;
   pasos = grados/GRADOS_POR_PASO;
   if(dir == 0){//movimiento horario
        for(i=0; i<pasos; i++){
          out(PARALLEL, 0x08); //1000
          Sleep(VALOR);
          i++;
          out(PARALLEL, 0x04); //0100
          Sleep(VALOR);
          i++;
          out(PARALLEL, 0x02); //0010
          Sleep(VALOR);
          i++;
          out(PARALLEL, 0x01); //0001
          Sleep(VALOR);
          i++;
        }
   }
   else{//movimiento antihorario
        for(i=0; i<pasos; i++){
          out(PARALLEL, 0x01); //0001
          Sleep(VALOR);
          i++;
          out(PARALLEL, 0x02); //0010
          Sleep(VALOR);
          i++;
          out(PARALLEL, 0x04); //0100
          Sleep(VALOR);
          i++;
          out(PARALLEL, 0x08); //1000
          Sleep(VALOR);
          i++;
        }
   }
}

HINSTANCE cargaLibreria(void){

    HINSTANCE hLib;
    hLib = LoadLibrary("inpout32.dll");

    if (hLib == NULL) {
        cout<<"Error en cargar la libreria! =/\n";
        cin.get();
        exit(-1);
        //return -1;
    }
    else {
       cout<<"Libreria cargada!\n";
       return hLib;
    }
}
