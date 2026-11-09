CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Alvos padrao
all: libminha.so usar_so

# Compilacao da biblioteca compartilhada
# -fPIC: Position Independent Code (essencial para bibliotecas compartilhadas ELF)
# -shared: indica ao linker para produzir um arquivo .so compartilhavel
libminha.so: MinhaLib/main.cpp MinhaLib/main.h
	$(CXX) $(CXXFLAGS) -fPIC -shared MinhaLib/main.cpp -o libminha.so

# Compilacao do consumidor
# -ldl: linka com a biblioteca libdl (dlopen, dlsym, dlclose, dlerror)
usar_so: UsarLib/main.cpp
	$(CXX) $(CXXFLAGS) UsarLib/main.cpp -o usar_so -ldl

clean:
	rm -f libminha.so usar_so

.PHONY: all clean
