SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

MKDIR = mkdir -p
RM = rm -f

TARGET = $(BIN_DIR)/Simple3D

SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

ifeq ($(shell uname -s), Linux)
    INC = $(shell sdl2-config --cflags)
    LIBS = $(shell sdl2-config --libs)
    
    LINK_FLAGS =
    EXT =
    
    DLL_COPY = @echo "No DLL for Linux."

else ifeq ($(OS),Windows_NT)
    
    SDL_PATH = SDL2
    INC = -I$(SDL_PATH)/include/SDL2
    
    LIBS = -L$(SDL_PATH)/lib -lmingw32 -lSDL2main -lSDL2
    
    LINK_FLAGS = -mwindows
    EXT = .exe
    
    DLL_COPY = cp $(SDL_PATH)/bin/SDL2.dll $(BIN_DIR)

else
    INC =
    LIBS = -lSDL2
    LINK_FLAGS =
    EXT =
    DLL_COPY = @echo "Not supported."

endif

TARGET := $(TARGET)$(EXT)

# Flags & Compiler
CXX = g++
CXXFLAGS = -Wall -O2 -MMD -MP $(INC)

# Target file name
TARGET = $(BIN_DIR)/Simple3D${EXT}

# Default Rule (> make)
all: $(TARGET)

# Executable from .o files, copies .dll file
$(TARGET): $(OBJS)
	$(MKDIR) $(BIN_DIR)
	$(CXX) $(LINK_FLAGS) -o $@ $^ $(LIBS)
	$(DLL_COPY)

# .o from .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Headers (para gerenciamento de dependências)
-include $(BUILD_DIR)/*.d

# Run Rule (> make run)
run: $(TARGET)
	./$(TARGET)

# Clean Rule (> make clean)
clean:
	$(RM) $(BUILD_DIR)/*.o
	$(RM) $(BUILD_DIR)/*.d
	$(RM) $(TARGET)