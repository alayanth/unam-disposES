#include <stdlib.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include<algorithm>
#include <sstream>

#define PARALLEL 0x378
#define REG_STATUS 0x379
#define GRADOS_POR_PASO 6.0
//3.6
#define VALOR 80

using namespace std;


//apuntadores a las funciones de la libreria
typedef short _stdcall (*inpfuncPtr)(short portaddr);
typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);
typedef BOOL (__stdcall *lpIsXP64Bit)(void);

//declaracion de funciones
void derecha(float grados);
void izquierda(float grad);
void moverMotor(float grados,int dir); //dirección 0 izquierda, dirección 1 derecha
string DecToBin(int number);

HINSTANCE cargaLibreria(void);
int leeEstado(void);


int main(){
    //cargaLibreria();
    //inpfuncPtr in;
    //oupfuncPtr out;
   //char datosent[4];
   char opt;
   int resultado;
   int status = 0;
   while(1){
      //system("CLS");
      resultado= 0;
      printf("Programa para mover un motor de pasos con el puerto paralelo"

             "\n\nPara salir presiona \"q\" \nPresiona una tecla para leer el registro de estados: ");
      cin>>opt;
      cout<<opt;
      if (opt == 'q'){ //Comparación para terminar el programa
        exit(0);
      }
      else{
        int status = leeEstado();
        std::stringstream ss;
        ss << "0x" << std::hex << status;
        cout << " hex> 0x" << std::hex << status;
        string s;
        s = DecToBin(status);
        cout << " bin> " << s;
        s = ss.str();
        s = s.substr(2,1);
        cout << "\n cut> " << s << "\n\n";
          resultado = s.compare("f"); //111 1
          if (resultado == 0){
            printf("1080 grados izquierda");
            izquierda(1080);
            //cin.get();
          }
          resultado = s.compare("1"); //000 1
          //resultado = strncmp(s,"000",3);
          if (resultado == 0){
             printf("720 grados derecha");
             derecha(720);
          }
          resultado = s.compare("b"); //101 1
          //resultado = strncmp(s,"101",3);
          if (resultado == 0){
             printf("180 grados izquierda");
             izquierda(180);
             //cin.get();
          }
          resultado = s.compare("d"); //110 1
          //resultado = strncmp(s,"110",3);
          if (resultado == 0){
             printf("270 grados derecha");
             derecha(270);
          }
      }
   }
}

void izquierda(float grados){
    HINSTANCE hLib = cargaLibreria();
    inpfuncPtr in;
    oupfuncPtr out;
    out = (oupfuncPtr) GetProcAddress(hLib, "Out32");
    if (out == NULL) {
        cout<<"GetProcAddress for Oup32 Failed.\n";
        cin.get();
        exit(-1);
    }
   float pasos=0;
   int i;
   pasos = grados/GRADOS_POR_PASO;
   printf("Son %f pasos",pasos);
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

void derecha(float grados){
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
   printf("Son %f pasos\n",pasos);
   for(i=0; i<pasos; i++){
      out(PARALLEL, 0x01); //0001
      Sleep(VALOR);
      //printf("Paso # %d\n",i);
      i++;
      out(PARALLEL, 0x02); //0010
      Sleep(VALOR);
      //printf("Paso # %d\n",i);
      i++;
      out(PARALLEL, 0x04); //0100
      Sleep(VALOR);
      //printf("Paso # %d\n",i);
      i++;
      out(PARALLEL, 0x08); //1000
      Sleep(VALOR);
      //printf("Paso # %d\n",i);
      i++;
   }
}

void moverMotor(float grados, int dir){
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
   if(dir = 0){
        for(i=0; i<pasos; i++){
          out(PARALLEL, 0x09); //1100
          Sleep(VALOR);
          printf("Paso # %d\n",i);
          out(PARALLEL, 0x05); //0110
          Sleep(VALOR);
          printf("Paso # %d\n",i);
          out(PARALLEL, 0x06); //0011
          Sleep(VALOR);
          printf("Paso # %d\n",i);
          out(PARALLEL, 0x0A); //1001
          Sleep(VALOR);
          printf("Paso # %d\n",i);
        }
   }
   else{
        for(i=0; i<pasos; i++){
          out(PARALLEL, 0x0A); //1100
          Sleep(VALOR);
          printf("Paso # %d\n",i);
          out(PARALLEL, 0x06); //0110
          Sleep(VALOR);
          printf("Paso # %d\n",i);
          out(PARALLEL, 0x05); //0011
          Sleep(VALOR);
          printf("Paso # %d\n",i);
          out(PARALLEL, 0x09); //1001
          Sleep(VALOR);
          printf("Paso # %d\n",i);
        }
   }
}

HINSTANCE cargaLibreria(void){

    HINSTANCE hLib;
    //lpIsXP64Bit gfpIsXP64Bit;

    hLib = LoadLibrary("inpout32.dll");

    if (hLib == NULL) {
        cout<<"Error en cargar la libreria! =/\n";
        cin.get();
        exit(-1);
        //return -1;
    }
    else {
       cout<<"Libreria cargada correctamente";
       return hLib;
       //gfpIsXP64Bit = (lpIsXP64Bit)GetProcAddress(hLib, "IsXP64Bit");
    }
}

int leeEstado(void){
    cout<<"\n\nLeyendo estado del puerto...";
    HINSTANCE hLib = cargaLibreria();
    int datain;
    inpfuncPtr in;
    in = (inpfuncPtr) GetProcAddress(hLib, "Inp32");
    if (in == NULL) {
        cout<<"GetProcAddress for Oup32 Failed.\n";
        cin.get();
        exit(EXIT_FAILURE);
    }
    datain = in(REG_STATUS);
    cout<< "\n\n datain: "<<datain << "\n";
    return datain;
}

string DecToBin(int number)
{
    string result = "";

    do
    {
        if ( (number & 1) == 0 )
            result += "0";
        else
            result += "1";

        number >>= 1;
    } while ( number );
    reverse(result.begin(), result.end());
    return result;
}
