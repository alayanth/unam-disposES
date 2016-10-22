#include <stdlib.h>
#include <string>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include<algorithm>
#include <sstream>

//Dirección de salida de datos del puerto paralelo
#define PARALLEL 0x378
//Dirección del registro de estados de puerto paralelo (entrada de datos)
#define REG_STATUS 0x379
//revisar muy bien cuantos grados da el motor por cada paso
#define GRADOS_POR_PASO 6.0
//Retraso para el movimiento del motor el sweet spot esta entre 50 y 100 
#define VALOR 80


/*Para enviar datos del puerto paralelo usar los pines 2, 3, 4 y 5
  Para la entrada de datos usar los pines 11, 10 y 12 en ese orden
  
  El pin 11 está invertido, es decir si se le manda un 1 entrara un 0 
  y viceversa.
  
  Dudas, reclamaciones y contribuciones at alayanth@gmail.com
*/

using namespace std;


//apuntadores a las funciones de la libreria
typedef short _stdcall (*inpfuncPtr)(short portaddr);
typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);
typedef BOOL (__stdcall *lpIsXP64Bit)(void);

//declaracion de funciones
void derecha(float grados);
void izquierda(float grad);

string DecToBin(int number);
HINSTANCE cargaLibreria(void); //para cargar la libreria DLL
int leeEstado(void);
void moverMotor(float grados,int dir);//dirección 0 izquierda, dirección 1 derecha ***EXPERIMENTAL****

int main(){
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
        int status = leeEstado(); //almacena el registro de estado
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
          }
          resultado = s.compare("1"); //000 1
          if (resultado == 0){
             printf("720 grados derecha");
             derecha(720);
          }
          resultado = s.compare("b"); //101 1
          if (resultado == 0){
             printf("180 grados izquierda");
             izquierda(180);
          }
          resultado = s.compare("d"); //110 1
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
/*Función experimental no se han hecho pruebas con ella, 
se agradecería si se puede probar el correcto 
funcionamiento de la misma*/
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
   else{
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
       cout<<"Libreria cargada correctamente";
       return hLib;
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
