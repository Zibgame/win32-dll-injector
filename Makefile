NAME = dll_injector.exe
DLL = bin/dll.dll
TARGET = bin/target.exe

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -Iincludes -Isrc/imgui -Isrc/imgui/backends

SRC = \
	src/main.cpp \
	src/injector/dll_injector.cpp \
	src/imgui/imgui.cpp \
	src/imgui/imgui_draw.cpp \
	src/imgui/imgui_tables.cpp \
	src/imgui/imgui_widgets.cpp \
	src/imgui/imgui_demo.cpp \
	src/imgui/backends/imgui_impl_win32.cpp \
	src/imgui/backends/imgui_impl_dx11.cpp

DLL_SRC = src/dll/dll.cpp
TARGET_SRC = src/target/main.cpp

OBJ = $(SRC:.cpp=.o)

LIBS = -ld3d11 -ldxgi -ld3dcompiler -lgdi32 -ldwmapi

all: $(NAME) $(DLL) $(TARGET)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LIBS)

$(DLL): $(DLL_SRC)
	if not exist bin mkdir bin
	CXXFLAGS += -m64
	$(CXX) -shared -o $(DLL) $(DLL_SRC) -luser32

$(TARGET): $(TARGET_SRC)
	if not exist bin mkdir bin
	$(CXX) $(TARGET_SRC) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q /S src\*.o 2>nul
	del /Q /S src\imgui\*.o 2>nul
	del /Q /S src\imgui\backends\*.o 2>nul
	del /Q /S src\injector\*.o 2>nul

fclean: clean
	del /Q $(NAME) 2>nul
	del /Q /S bin\*.exe 2>nul
	del /Q /S bin\*.dll 2>nul

re: fclean all