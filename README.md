# Carregamento Dinâmico de DLLs no Windows

Atividade prática desenvolvida para a disciplina de **Sistemas Operacionais (SO)** do curso de **Bacharelado em Ciência da Computação** — Instituto Federal Catarinense (IFC).

---

## 🎯 Objetivo

Compreender e demonstrar na prática os conceitos de **ligação dinâmica (*Dynamic Linking*)** e **carregamento explícito em tempo de execução (*Explicit Runtime Dynamic Loading*)** sob o sistema operacional Windows, observando o ciclo de vida de mapeamento de módulos no espaço de endereçamento de um processo.

---

## 🧠 Conceitos Abordados

- **Bibliotecas de Ligação Dinâmica (DLL - *Dynamic Link Library*):** Código modular compilado e compartilhado que pode ser carregado sob demanda, economizando memória principal e permitindo atualizações de componentes sem a necessidade de recompilar a aplicação principal.
- **Exportação de Símbolos:** Uso de `__declspec(dllexport)` e convenção de ligação C (`extern "C"`) para evitar o *name mangling* (decoração de nomes) do C++, facilitando a resolução de símbolos em tempo de execução.
- **Ciclo de Vida com Win32 API:**
  - `LoadLibraryA`: Solicita ao Sistema Operacional que carregue o módulo `.dll` e mapeie suas seções de código e dados no espaço de endereçamento virtual do processo chamador.
  - `GetProcAddress`: Realiza a resolução de símbolos em tempo de execução, consultando a tabela de exportação da DLL para obter o ponteiro de memória da função desejada.
  - `FreeLibrary`: Decrementa a contagem de referências da biblioteca; quando atinge zero, o Sistema Operacional desmapeia o módulo da memória do processo (`DLL_PROCESS_DETACH`).

---

## 📁 Estrutura do Projeto

O projeto é dividido em dois módulos desenvolvidos em C++ com o auxílio do Code::Blocks / MinGW:

```text
SO_DLL/
├── MinhaDLL/                     # Módulo responsável por gerar a DLL
│   ├── main.h                    # Declaração e macros de exportação/importação (DLL_EXPORT)
│   ├── main.cpp                  # Implementação da função exportada e ponto de entrada (DllMain)
│   ├── MinhaDLL.cbp              # Arquivo de projeto do Code::Blocks
│   └── bin/Debug/
│       ├── MinhaDLL.dll          # Biblioteca dinâmica gerada
│       ├── libMinhaDLL.a         # Biblioteca de importação (caso usada em ligação implícita)
│       └── libMinhaDLL.def       # Tabela de definições de símbolos exportados
│
└── UsarDLL/                      # Aplicação consumidora em modo console
    ├── main.cpp                  # Carregamento dinâmico via LoadLibrary e GetProcAddress
    ├── UsarDLL.cbp               # Arquivo de projeto do Code::Blocks
    └── bin/Debug/
        ├── MinhaDLL.dll          # Cópia da DLL necessária para a execução local
        └── UsarDLL.exe           # Executável do programa de teste
```

---

## 🔍 Funcionamento Detalhado

### 1. `MinhaDLL`
- **Ponto de Entrada (`DllMain`):** Monitora eventos de inicialização e finalização do processo/thread (`DLL_PROCESS_ATTACH`, `DLL_PROCESS_DETACH`).
- **Função Exportada (`SomeFunction`):** Recebe uma string (`const LPCSTR`) e exibe uma caixa de diálogo nativa utilizando a API `MessageBoxA` da Win32:
  ```cpp
  void DLL_EXPORT SomeFunction(const LPCSTR sometext)
  {
      MessageBoxA(0, sometext, "DLL Message", MB_OK | MB_ICONINFORMATION);
  }
  ```

### 2. `UsarDLL`
A aplicação interage com o usuário por meio de etapas pausadas no console, permitindo acompanhar o estado do processo em ferramentas de inspeção do SO (como o Gerenciador de Tarefas ou Process Explorer):
1. **Pausa inicial:** O processo é iniciado sem a biblioteca na memória.
2. **Carregamento:** `LoadLibraryA("MinhaDLL.dll")` é chamado, mapeando a DLL na memória do processo.
3. **Resolução de Símbolos:** `GetProcAddress(dll, "SomeFunction")` localiza o endereço da função na memória e realiza o cast para o tipo de ponteiro `dllF`.
4. **Execução:** A função é executada, disparando o `MessageBoxA("Ola mundo!")`.
5. **Liberação:** `FreeLibrary(dll)` descarrega a DLL, liberando o espaço de memória ocupado pelo módulo.

---

## 🚀 Como Executar

### Pré-requisitos
- Sistema operacional Windows (ou ambiente Linux com [Wine](https://www.winehq.org/)).
- IDE [Code::Blocks](https://www.codeblocks.org/) com compilador MinGW/GCC (ou compilação direta via linha de comando com `g++`).

### Executando o binário já compilado
1. Navegue até o diretório `UsarDLL/bin/Debug/`:
   ```cmd
   cd UsarDLL\bin\Debug
   UsarDLL.exe
   ```
2. Pressione `Enter` em cada uma das etapas para acompanhar o fluxo de carregamento, chamada e liberação da DLL.

### Compilando via linha de comando (MinGW / GCC)
Caso deseje recompilar os projetos sem o Code::Blocks:

1. **Compilar a DLL:**
   ```cmd
   cd MinhaDLL
   g++ -shared -DBUILD_DLL -o bin\Debug\MinhaDLL.dll main.cpp -Wl,--out-implib,bin\Debug\libMinhaDLL.a
   ```

2. **Compilar o Executável:**
   ```cmd
   cd ..\UsarDLL
   copy ..\MinhaDLL\bin\Debug\MinhaDLL.dll bin\Debug\MinhaDLL.dll
   g++ -o bin\Debug\UsarDLL.exe main.cpp
   ```

---

## 👨‍💻 Autor

- **Carlos Garrard Fontanive**
- Bacharelado em Ciência da Computação — IFC Campus Rio do Sul
