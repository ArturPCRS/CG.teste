# Makefile para Windows com MinGW-w64

PROG = ProgOpenGL.exe

FONTES = Ponto.cpp \
         Poligono.cpp \
         Temporizador.cpp \
         ListaDeCoresRGB.cpp \
         Triangulo.cpp \
         Quadtree.cpp \
         Consulta.cpp \
         Prog-Triangulos.cpp

OBJETOS = $(FONTES:.cpp=.o)

CXX = g++

CXXFLAGS = -std=c++14 \
           -MMD -MP \
           -g \
           -Iinclude/GL \
           -Wno-write-strings \
           -Wno-narrowing \
           -DWIN32

LDFLAGS = -Llib/x64

LDLIBS = -lfreeglut \
         -lopengl32 \
         -lglu32 \
         -lm

.PHONY: all clean run

all: $(PROG)

$(PROG): $(OBJETOS)
	$(CXX) $(OBJETOS) -o $@ $(LDFLAGS) $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(PROG)
	./$(PROG)

clean:
	cmd /C "del /Q $(OBJETOS) $(OBJETOS:.o=.d) $(PROG) 2>NUL"

# recompila os .cpp quando algum .h incluido muda
-include $(OBJETOS:.o=.d)
