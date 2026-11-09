#include <iostream>
#include <Windows.h>
#include <string>

using namespace std;

typedef void (*dllF)(LPCSTR);

int main()
{
    string linha;

    cout << "Aperte Enter para carregar DLL" << endl;
    getline(cin, linha);

    HINSTANCE dll = LoadLibraryA("MinhaDLL.dll");

    if (dll != NULL)
    {
        cout << "Aperte Enter para buscar a funcao" << endl;
        getline(cin, linha);

        dllF func = (dllF)GetProcAddress(dll, "SomeFunction");

        if (func != NULL)
        {
            cout << "Funcao SomeFunction encontrada!" << endl;
            func("Ola mundo!");
        }
        else
        {
            cout << "Erro ao procurar a funcao" << endl;
        }

        cout << "Aperte Enter fechar a biblioteca" << endl;
        getline(cin, linha);

        FreeLibrary(dll);
    }
    else
    {
        cout << "Nao foi possivel carregar a DLL" << endl;
    }

    cout << "Aperte Enter para sair" << endl;
    getline(cin, linha);

    return 0;
}
