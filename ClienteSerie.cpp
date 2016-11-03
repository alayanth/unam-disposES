#include <stdio.h>
#include <iostream>
#include <string.h>
#include <windows.h>
#include <sstream>

using namespace std;

int main(int argc, char **argv) {
    int i;
    int grados;
    int sentido;
    int cSentido;
    int ch;
    int comma = ',';
    int last = '!';
    
    HANDLE file;
    COMMTIMEOUTS timeouts;
    DWORD read, written;
    DCB port;
    HANDLE keyboard = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    char port_name[128] = "\\\\.\\COM1"; // editar si el COM no es este
    char init[] = "";

    cout << "Programa para controlar un motor de pasos a traves del puerto paralelo y serie"
         << "\n\n Ingresa los grados que se movera el motor: ";
    cin >> grados;
    cout << "\n Ingresa el sentido (1 horario, 0 antihorario): ";
    cin >> sentido;
    cout << "  Grados: " << grados;
    switch(sentido){
        case 0:
            cout << "\n  Sentido: antihorario\n";
            break;
        case 1:
            cout << "\n  Sentido: horario\n";
            break;
        default:
            cout << "\n  Sentido invalido!";
            exit(0);
            break;
    }
    //transformación de float a string
    std::stringstream ss;
    ss << grados;
    std::string sGrados(ss.str());
    const char *cGrados = sGrados.c_str();

    // Abrimos el puerto COM y lo configuramos
    file = CreateFile(port_name,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if ( INVALID_HANDLE_VALUE == file) {
        fprintf(stderr,"Error abriendo el puerto");
        return 1;
    }

    // get the current DCB, and adjust a few bits to our liking.
    memset(&port, 0, sizeof(port));
    port.DCBlength = sizeof(port);
    if ( !GetCommState(file, &port))
        fprintf(stderr,"getting comm state");
    //Configuración del puerto
    // !!!!!!!!!!Debe see igual en el cliente como en el servidor!!!!!!
    port.BaudRate = CBR_9600;
    port.ByteSize = 8;
    port.StopBits = ONESTOPBIT;
    port.Parity = NOPARITY;
    if (!SetCommState(file, &port))
        fprintf(stderr,"Error en los ajustes del puerto");

    //timeouts para lectura y escritura
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

    //Convierte los datos tomados en el programa y los envia como chars sobre
    //el puerto serie
    //Envia los grados char a char
    for(i=0;i < strlen(cGrados); i++){
        WriteFile(file,&cGrados[i], 1, &written, NULL);
    }
    //caracter de separación, sirve para saber donde cortar la cadena e identificar parametros
    WriteFile(file,&comma, 1, &written, NULL);
    switch(sentido){
        case 0:
            cSentido = '0';
            WriteFile(file,&cSentido, 1, &written, NULL); //envia el sentido del giro
            break;
        case 1:
            cSentido = '1';
            WriteFile(file,&cSentido, 1, &written, NULL); 
            break;
    }
    WriteFile(file,&last, 1, &written, NULL); // caracter de control para finalizar nuestro envio de información

    // cierra handlers y vamonos!!
    CloseHandle(keyboard);
    CloseHandle(file);
    return 0;
}
