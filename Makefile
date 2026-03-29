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

# ✅ ICON FIX
ICON_RC = src/img/icon.rc
ICON_RES = src/img/icon.res

LIBS = -ld3d11 -ldxgi -ld3dcompiler -lgdi32 -ldwmapi

GREEN=\033[1;32m
YELLOW=\033[1;33m
RED=\033[1;31m
CYAN=\033[1;36m
RESET=\033[0m

all: $(NAME) $(DLL) $(TARGET)

# =========================
# EXE
# =========================
$(NAME): $(OBJ) $(ICON_RES)
	@echo $(CYAN)[INFO] Killing running injector...$(RESET)
	-taskkill /F /IM dll_injector.exe >nul 2>&1
	@echo $(GREEN)[BUILD] Compiling injector...$(RESET)
	$(CXX) $(OBJ) $(ICON_RES) -o $(NAME) $(LIBS)
	@echo $(GREEN)[OK] Injector built successfully$(RESET)

# =========================
# ICON (.rc → .res)
# =========================
$(ICON_RES): $(ICON_RC)
	@echo $(YELLOW)[COMPILING] icon.rc$(RESET)
	windres $(ICON_RC) -O coff -o $(ICON_RES)

# =========================
# DLL
# =========================
$(DLL): $(DLL_SRC)
	@echo $(CYAN)[INFO] Cleaning DLL locks...$(RESET)
	-taskkill /F /IM target.exe >nul 2>&1
	-del /Q $(DLL) >nul 2>&1
	@if not exist bin mkdir bin
	@echo $(GREEN)[BUILD] Compiling DLL...$(RESET)
	$(CXX) -shared -o $(DLL) $(DLL_SRC) -luser32
	@echo $(GREEN)[OK] DLL built successfully$(RESET)

# =========================
# TARGET
# =========================
$(TARGET): $(TARGET_SRC)
	@echo $(CYAN)[INFO] Building target process...$(RESET)
	@if not exist bin mkdir bin
	$(CXX) $(TARGET_SRC) -o $(TARGET)
	@echo $(GREEN)[OK] Target built successfully$(RESET)

# =========================
# OBJ
# =========================
%.o: %.cpp
	@echo $(YELLOW)[COMPILING] $<$(RESET)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# =========================
# CLEAN
# =========================
clean:
	@echo $(RED)[CLEAN] Removing object files...$(RESET)
	-del /Q /S src\*.o >nul 2>&1
	-del /Q /S src\imgui\*.o >nul 2>&1
	-del /Q /S src\imgui\backends\*.o >nul 2>&1
	-del /Q /S src\injector\*.o >nul 2>&1
	-del /Q $(ICON_RES) >nul 2>&1

fclean: clean
	@echo $(RED)[FCLEAN] Removing binaries...$(RESET)
	-del /Q $(NAME) >nul 2>&1
	-del /Q /S bin\*.exe >nul 2>&1
	-del /Q /S bin\*.dll >nul 2>&1

re: fclean all