#include <stdio.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <sstream>

//Dirección de salida de datos del puerto paralelo
#define PARALLEL 0x378
//Dirección del registro de estados de puerto paralelo (entrada de datos)
#define REG_STATUS 0x379
//revisar muy bien cuantos grados da el motor por cada paso
#define GRADOS_POR_PASO 6.0
//Retraso para el movimiento del motor el sweet spot esta entre 50 y 100
#define VALOR 80

using namespace std;

typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);

void moverMotor(int grados,int dir);
HINSTANCE cargaLibreria(void);

int main(int argc, char **argv) {
    char command[100];
    char buffer [1];
    const char *direction;
    char port_name[128] = "\\\\.\\COM1";
    char init[] = ""; // e.g., "ATZ" to completely reset a modem.

    int stringBuffer[15];
    int sentido;
    int i = 0;
    int ch;
    stringstream ss;
    string degrees;

    HANDLE file;
    COMMTIMEOUTS timeouts;
    DWORD read, written;
    DCB port;
    HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;


    if ( argc > 2 )
        sprintf(port_name, "\\\\.\\COM%c", argv[1][0]);

    // open the comm port.
    file = CreateFile(port_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if ( INVALID_HANDLE_VALUE == file) {
        fprintf(stderr," error opening file");
        return 1;
    }

    // get the current DCB, and adjust a few bits to our liking.
    memset(&port, 0, sizeof(port));
    port.DCBlength = sizeof(port);
    if ( !GetCommState(file, &port))
        fprintf(stderr,"bad getting comm state");

    port.BaudRate = CBR_9600;
    port.ByteSize = 8;
    port.StopBits = ONESTOPBIT;
    port.Parity = NOPARITY;
    //if (!BuildCommDCB("baud=19200 parity=n data=8 stop=1", &port))
    //    fprintf(stderr,"building comm DCB");
    if (!SetCommState(file, &port))
        fprintf(stderr,"bad port settings");



    // set short timeouts on the comm port.
    timeouts.ReadIntervalTimeout = 1;
    timeouts.ReadTotalTimeoutMultiplier = 1;
    timeouts.ReadTotalTimeoutConstant = 1;
    timeouts.WriteTotalTimeoutMultiplier = 1;
    timeouts.WriteTotalTimeoutConstant = 1;
    if (!SetCommTimeouts(file, &timeouts))
        fprintf(stderr,"setting port time-outs.");

    // set keyboard to raw reading.
    if (!GetConsoleMode(keyboard, &mode))
        fprintf(stderr,"getting keyboard mode");
    mode &= ~ ENABLE_PROCESSED_INPUT;
    if (!SetConsoleMode(keyboard, mode))
        fprintf(stderr,"setting keyboard mode");

    if (!EscapeCommFunction(file, CLRDTR))
        fprintf(stderr,"clearing DTR");
    Sleep(200);
    if (!EscapeCommFunction(file, SETDTR))
        fprintf(stderr,"setting DTR");

    if ( !WriteFile(file, init, sizeof(init), &written, NULL))
        fprintf(stderr,"writing data to port");

    if (written != sizeof(init))
        fprintf(stderr,"not all data written to port");

    //loop de ejecución
    do {
        // check for data on port and display it on screen.
        ReadFile(file, buffer, sizeof(buffer), &read, NULL);
        if ( read ){
            if(buffer[0] != '!'){
                WriteFile(screen, &buffer, read, &written, NULL);
                ch = buffer[0];
                if (ch != 0){
                    switch(ch){
                        case 48:
                            stringBuffer[i] = 0;
                            break;
                        case 49:
                            stringBuffer[i] = 1;
                            break;
                        case 50:
                            stringBuffer[i] = 2;
                            break;
                        case 51:
                            stringBuffer[i] = 3;
                            break;
                        case 52:
                            stringBuffer[i] = 4;
                            break;
                        case 53:
                            stringBuffer[i] = 5;
                            break;
                        case 54:
                            stringBuffer[i] = 6;
                            break;
                        case 55:
                            stringBuffer[i] = 7;
                            break;
                        case 56:
                            stringBuffer[i] = 8;
                            break;
                        case 57:
                            stringBuffer[i] = 9;
                            break;
                        default:
                            stringBuffer[i] = ch;
                            break;
                    }
                    i++;
                }
            }
        }
    } while (buffer[0] != '!');

    int tIntArray = i;
    //cout << "\n Tamano del array: " << tIntArray;
    for (i = 0; i < tIntArray; ++i){
        if(stringBuffer[i] == 44){
            cout << "\nCOMMA!!";
            cout << "\nSiguiente caracter: " << stringBuffer[i+1];
            switch(stringBuffer[i+1]){
                case 1:
                    sentido = 1;
                    break;
                case 0:
                    sentido = 0;
                    break;
            }
        }
    }
    /*
    cout << "\n";
    for(i=0; i < tIntArray; ++i)
        cout << stringBuffer[i] << " ";
    */
    cout << "\n";
    for (i=0; i < tIntArray-2; ++i)
        ss << stringBuffer[i];
    int grados;
    ss >> grados;
    cout << "\n|||Parametros para mover el motor recibidos!|||"
    cout << "\nSentido: " << sentido;
    cout << "\nGrados: "<< grados;
    moverMotor(grados,sentido);

    Sleep(50000);
    //cerrando handlers y vamonos
    //CloseHandle(keyboard)
    CloseHandle(file);
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
   if(dir == 0){
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
