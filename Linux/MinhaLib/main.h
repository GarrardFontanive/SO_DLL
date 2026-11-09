#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

// Define visibilidade padrão para garantir que o símbolo seja exportado na tabela dinâmica do ELF
#define SO_EXPORT __attribute__((visibility("default")))

void SO_EXPORT SomeFunction(const char* sometext);

#ifdef __cplusplus
}
#endif

#endif // MAIN_H
