#include <iostream>
#include <windows.h>
#include <stdio.h>
#define PARALLEL 0x378

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
    int x = 0x0;
    do{
        cout<<"\nIntroduce un valor decimal entre 0 y 255 (para salir usa 256) \n";
        cin>>x;
        //std::stringstream stream;
        //stream << "0x"  << std::hex << x;
        //stream >> x;
        cout<< "\nHex: " << "0x" <<std::hex<< x;
        out(PARALLEL,x);
    }
    while (x != 256);

    return 0;
}
