#include "main.h"
#include <iostream>

// Atributo constructor: executado automaticamente pelo carregador dinâmico do SO (ld.so)
// quando o módulo é mapeado na memória do processo (equivalente ao DLL_PROCESS_ATTACH no Windows)
__attribute__((constructor))
void inicializar_biblioteca() {
    std::cout << "[libminha.so] Constructor: biblioteca mapeada no processo (equivalente a DLL_PROCESS_ATTACH)." << std::endl;
}

// Atributo destructor: executado automaticamente quando o módulo é descarregado
// da memória do processo (equivalente ao DLL_PROCESS_DETACH no Windows)
__attribute__((destructor))
void finalizar_biblioteca() {
    std::cout << "[libminha.so] Destructor: biblioteca descarregada da memoria (equivalente a DLL_PROCESS_DETACH)." << std::endl;
}

void SO_EXPORT SomeFunction(const char* sometext) {
    std::cout << "\n========================================" << std::endl;
    std::cout << " [libminha.so] Executando funcao exportada!" << std::endl;
    std::cout << " Mensagem: " << (sometext ? sometext : "(vazio)") << std::endl;
    std::cout << "========================================\n" << std::endl;
}
