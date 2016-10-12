/*
Este programa hace uso del puerto paralelo para encender un display de 7 segmentos y también puede usarse para escribir datos a una memoria RAM
+/

#include <iostream>
#include <windows.h>
#include <stdio.h>

#define PARALLEL 0x378 //dirección del puerto paralelo

using namespace std;

typedef short _stdcall (*inpfuncPtr)(short portaddr);
typedef void _stdcall (*oupfuncPtr)(short portaddr, short datum);
typedef BOOL	(__stdcall *lpIsXP64Bit)(void);

int main(){
    HINSTANCE hLib;
    inpfuncPtr in;
    oupfuncPtr out;
    lpIsXP64Bit gfpIsXP64Bit;

    hLib = LoadLibrary("inpout32.dll");

    if (hLib == NULL) {
        cout<<"Error en cargar la libreria! =/\n";
        cin.get();
        return -1;
    }
    else {
       cout<<"Libreria cargada correctamente";
       gfpIsXP64Bit = (lpIsXP64Bit)GetProcAddress(hLib, "IsXP64Bit");
    }
    in = (inpfuncPtr) GetProcAddress(hLib, "Inp32");
    if (in == NULL) {
        cout<<"GetProcAddress for Inp32 Failed.\n";
        cin.get();
        return -1;
    }
    out = (oupfuncPtr) GetProcAddress(hLib, "Out32");
    if (out == NULL) {
        cout<<"GetProcAddress for Oup32 Failed.\n";
        cin.get();
        return -1;
    }
    char x;
	int y;
    do{
        cout<<"\nIntroduce un caracter (para salir usa 0) \n";
        cin>>x;
		switch(x){
			case 'a': case 'A':
				y = 136;
				break;
			case 'b': case 'B':
				y = 131;
				break;
            case 'c': case 'C':
				y = 198;
				break;
            case 'd': case 'D':
				y = 161;
				break;
            case 'e': case 'E':
				y = 134;
				break;
            case 'f': case 'F':
				y = 142;
				break;
            case 'g': case 'G':
				y = 144;
				break;
            case 'h': case 'H':
				y = 137;
				break;
            case 'i': case 'I':
				y = 207;
				break;
            case 'j': case 'J':
				y = 241;
				break;
            case 'k': case 'K':
				y = 133;
				break;
            case 'l': case 'L':
				y = 199;
				break;
            case 'm': case 'M':
				y = 176;
				break;
            case 'n': case 'N':
				y = 171;
				break;
            case 'o': case 'O':
				y = 163;
				break;
            case 'p': case 'P':
				y = 140;
				break;
            case 'q': case 'Q':
				y = 152;
				break;
            case 'r': case 'R':
				y = 175;
				break;
            case 's': case 'S':
				y = 146;
				break;
            case 't': case 'T':
				y = 135;
				break;
            case 'u': case 'U':
				y = 227;
				break;
            case 'v': case 'V':
				y = 193;
				break;
            case 'w': case 'W':
				y = 226;
				break;
            case 'x': case 'X':
				y = 143;
				break;
            case 'y': case 'Y':
				y = 145;
				break;
            case 'z': case 'Z':
				y = 182;
				break;
            case '.':
				y = 127;
				break;
            case '-':
                y= 191;
                break;
            case '0':
				y = 192;
				break;
            case '1':
				y = 249;
				break;
            case '2':
				y = 164;
				break;
            case '3':
				y = 176;
				break;
            case '4':
				y = 153;
				break;
            case '5':
				y = 146;
				break;
            case '6':
				y = 130;
				break;
            case '7':
				y = 248;
				break;
            case '8':
				y = 128;
				break;
            case '9':
				y = 152;
				break;
		}
        cout<< "\nHex: " << "0x" <<std::hex<< y;
        out(PARALLEL,y);
    }
    while (x != 0);

    return 0;
}
