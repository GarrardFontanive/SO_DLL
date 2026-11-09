# Carregamento Dinâmico de Bibliotecas: Windows (DLL) vs. Linux (Shared Object)

Atividade prática desenvolvida para a disciplina de **Sistemas Operacionais (SO)** do curso de **Bacharelado em Ciência da Computação** — Instituto Federal Catarinense (IFC).

---

## 🎯 Objetivo

Compreender e demonstrar na prática os conceitos de **ligação dinâmica (*Dynamic Linking*)** e **carregamento explícito em tempo de execução (*Explicit Runtime Dynamic Loading*)** em dois dos principais sistemas operacionais do mercado: **Windows** (`.dll`) e **Linux** (`.so`).

O projeto permite comparar o comportamento do **carregador do SO (*loader*)**, o mapeamento de páginas no **espaço de endereçamento virtual** do processo e o ciclo de vida dos módulos de código compartilhado.

---

## ⚖️ Comparativo entre Sistemas Operacionais

| Característica | Windows (Win32 API) | Linux (POSIX / `libdl`) |
| :--- | :--- | :--- |
| **Extensão do Módulo** | `.dll` (*Dynamic Link Library*) | `.so` (*Shared Object*) |
| **Formato de Binário** | PE/COFF (*Portable Executable*) | ELF (*Executable and Linkable Format*) |
| **Código Relocável** | Base Relocation / ImageBase | `-fPIC` (*Position Independent Code*) |
| **Carregar Módulo** | `LoadLibraryA("MinhaDLL.dll")` | `dlopen("./libminha.so", RTLD_LAZY)` |
| **Resolver Função** | `GetProcAddress(handle, "nome")` | `dlsym(handle, "nome")` |
| **Descarregar Módulo** | `FreeLibrary(handle)` | `dlclose(handle)` |
| **Checagem de Erros** | `GetLastError()` | `dlerror()` |
| **Ponto de Entrada/Saída** | `DllMain(...)` (`DLL_PROCESS_ATTACH/DETACH`) | `__attribute__((constructor))` e `((destructor))` |
| **Inspeção de Memória** | Process Explorer, VMMap, Process Hacker | `/proc/<PID>/maps`, `pmap <PID>` |

---

## 📁 Estrutura do Repositório

```text
SO_DLL/
├── MinhaDLL/                     # [Windows] Projeto da DLL
│   ├── main.h                    # Declaração e macros de exportação (DLL_EXPORT)
│   ├── main.cpp                  # Função com MessageBoxA e DllMain
│   ├── MinhaDLL.cbp              # Projeto Code::Blocks
│   └── bin/Debug/
│       ├── MinhaDLL.dll          # Biblioteca dinâmica gerada
│       ├── libMinhaDLL.a         # Biblioteca de importação
│       └── libMinhaDLL.def       # Tabela de exportação de símbolos
│
├── UsarDLL/                      # [Windows] Aplicação consumidora
│   ├── main.cpp                  # Carregamento explícito via LoadLibraryA e GetProcAddress
│   ├── UsarDLL.cbp               # Projeto Code::Blocks
│   └── bin/Debug/
│       ├── MinhaDLL.dll          # DLL posicionada no mesmo diretório do executável
│       └── UsarDLL.exe           # Executável de teste
│
├── Linux/                        # [Linux] Implementação equivalente em POSIX
│   ├── MinhaLib/
│   │   ├── main.h                # Declaração com visibilidade padrão ELF
│   │   └── main.cpp              # Implementação com constructor e destructor do GCC
│   ├── UsarLib/
│   │   └── main.cpp              # Consumidor via dlopen, dlsym e dlclose com PID
│   ├── Makefile                  # Script de automação de compilação
│   ├── libminha.so               # Biblioteca compartilhada gerada
│   └── usar_so                   # Executável do consumidor Linux
│
├── .gitignore                    # Filtro de arquivos intermediários (*.o, *.layout, etc.)
└── README.md                     # Documentação completa
```

---

## 🔍 Detalhamento das Implementações

### 1. Implementação no Windows
- **`MinhaDLL/main.cpp`:**
  - Exporta `SomeFunction` via `__declspec(dllexport)` e convenção `extern "C"` (evitando *name mangling* do C++).
  - Executa uma caixa de diálogo nativa do Windows através da API `MessageBoxA`.
  - Ponto de entrada padrão `DllMain` monitorando eventos `DLL_PROCESS_ATTACH` e `DLL_PROCESS_DETACH`.
- **`UsarDLL/main.cpp`:**
  - Aplicação console que pausa a cada etapa (`cin.getline`), permitindo abrir o Gerenciador de Tarefas ou Process Explorer para ver a DLL sendo mapeada e posteriormente desmapeada da memória do processo com `FreeLibrary`.

---

### 2. Implementação no Linux (Arch Linux / POSIX)
- **`Linux/MinhaLib/main.cpp`:**
  - Compilada com a flag `-fPIC` (*Position Independent Code*), essencial para bibliotecas dinâmicas no formato ELF, permitindo que as páginas de código físico sejam mapeadas em qualquer endereço virtual sem relocalização em tempo de carga.
  - Utiliza os atributos `__attribute__((constructor))` e `__attribute__((destructor))` do GCC/Clang, que atuam de forma análoga a `DLL_PROCESS_ATTACH` e `DLL_PROCESS_DETACH`.
- **`Linux/UsarLib/main.cpp`:**
  - Utiliza a biblioteca POSIX `<dlfcn.h>` (`-ldl`).
  - Imprime o **PID** do processo atual e pausa em cada fase para verificação empírica dos mapas de memória virtual do Linux.

---

## 🧪 Experimento de Sistemas Operacionais no Linux

Um dos conceitos mais importantes em Sistemas Operacionais é o **espaço de endereçamento virtual** e a tabela de páginas de memória do processo.

Para comprovar o mapeamento dinâmico em tempo real no Linux:

1. **Abra um terminal** e execute o programa:
   ```bash
   cd Linux
   ./usar_so
   ```
   O programa exibirá o **PID** (exemplo: `100793`) e aguardará na etapa `[1]`.

2. **Abra um segundo terminal** e inspecione a memória virtual do processo:
   ```bash
   cat /proc/100793/maps | grep libminha
   ```
   *Resultado:* Nenhuma saída (a biblioteca ainda não existe no espaço de endereçamento).

3. No primeiro terminal, **pressione Enter** para executar `dlopen()`.

4. No segundo terminal, execute o comando novamente:
   ```bash
   cat /proc/100793/maps | grep libminha
   ```
   *Resultado:* O kernel agora exibe as páginas da `libminha.so` mapeadas no processo com suas respectivas permissões:
   - `r--p` (seção de cabeçalhos e constantes)
   - `r-xp` (seção de código executável `.text`)
   - `rw-p` (seção de dados `.data`)

5. No primeiro terminal, avance para a etapa de chamada da função e depois para `dlclose()`.

6. Verifique o mapa de memória novamente: as entradas foram desmapeadas pelo Sistema Operacional!

---

## 🚀 Como Compilar e Executar

### No Linux (Arch Linux / qualquer distribuição com GCC):
Basta usar o `make` já configurado:
```bash
cd Linux
make              # Compila a libminha.so (-fPIC -shared) e o executável usar_so (-ldl)
./usar_so         # Executa o programa interativo
```
Para limpar os binários compilados:
```bash
make clean
```

### No Windows:
1. Abra os arquivos de projeto `.cbp` no **Code::Blocks** (`MinhaDLL/MinhaDLL.cbp` e `UsarDLL/UsarDLL.cbp`) e compile.
2. Ou via linha de comando com MinGW:
   ```cmd
   cd MinhaDLL
   g++ -shared -DBUILD_DLL -o bin\Debug\MinhaDLL.dll main.cpp -Wl,--out-implib,bin\Debug\libMinhaDLL.a

   cd ..\UsarDLL
   copy ..\MinhaDLL\bin\Debug\MinhaDLL.dll bin\Debug\MinhaDLL.dll
   g++ -o bin\Debug\UsarDLL.exe main.cpp
   ```
3. Execute `UsarDLL\bin\Debug\UsarDLL.exe`.

---

## 👨‍💻 Autor

- **Carlos Garrard Fontanive**
- Bacharelado em Ciência da Computação — IFC Campus Rio do Sul
