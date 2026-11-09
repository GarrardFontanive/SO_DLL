#include <iostream>
#include <string>
#include <dlfcn.h>
#include <unistd.h>

using namespace std;

typedef void (*soF)(const char*);

int main() {
    string linha;
    pid_t pid = getpid();

    cout << "==========================================================" << endl;
    cout << "  Demonstracao de Shared Object (.so) no Linux - SO" << endl;
    cout << "==========================================================" << endl;
    cout << "PID do Processo atual: " << pid << endl;
    cout << "Dica para SO: abra outro terminal e execute para ver a memoria:" << endl;
    cout << "  cat /proc/" << pid << "/maps | grep libminha" << endl;
    cout << "----------------------------------------------------------" << endl;

    cout << "\n[1] Pressione ENTER para carregar a biblioteca via dlopen()...";
    getline(cin, linha);

    // Carregamento dinamico com flags POSIX:
    // RTLD_LAZY: resolve simbolos sob demanda a medida que o codigo e executado
    void* handle = dlopen("./libminha.so", RTLD_LAZY);

    if (!handle) {
        cerr << "Erro ao carregar a biblioteca: " << dlerror() << endl;
        return 1;
    }

    cout << "-> Biblioteca carregada com sucesso! (Verifique /proc/" << pid << "/maps no outro terminal)" << endl;

    cout << "\n[2] Pressione ENTER para buscar a funcao com dlsym()...";
    getline(cin, linha);

    // Limpa qualquer erro anterior retornado por dlfcn
    dlerror();

    // Resolucao dinamica do simbolo em tempo de execucao
    soF func = (soF)dlsym(handle, "SomeFunction");
    const char* dlsym_error = dlerror();

    if (dlsym_error) {
        cerr << "Erro ao localizar simbolo: " << dlsym_error << endl;
        dlclose(handle);
        return 1;
    }

    cout << "-> Funcao 'SomeFunction' encontrada no endereco: " << (void*)func << endl;
    cout << "-> Chamando a funcao da biblioteca compartilhada..." << endl;
    func("Ola mundo direto do Arch Linux!");

    cout << "\n[3] Pressione ENTER para descarregar a biblioteca com dlclose()...";
    getline(cin, linha);

    // Descarrega o modulo da memoria virtual do processo
    dlclose(handle);
    cout << "-> dlclose() executado! (Verifique /proc/" << pid << "/maps para constatar o desmapeamento)" << endl;

    cout << "\n[4] Pressione ENTER para encerrar o programa...";
    getline(cin, linha);

    return 0;
}
