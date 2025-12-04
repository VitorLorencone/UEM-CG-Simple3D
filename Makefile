# OS pick
ifeq ($(OS),Windows_NT)
	MKDIR = mkdir -p
    RM = rm -f
    EXT = .exe
else
	MKDIR = mkdir -p
    RM = rm -f
    EXT =
endif

# SDL2 path
SDL_PATH = SDL2
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# project files
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC))
#OBJS = main.o edit_distance.o duktape.o

# Includes e libs do SDL2
INC = -I$(SDL_PATH)/include/SDL2
LIBS = -L$(SDL_PATH)/lib -lmingw32 -lSDL2main -lSDL2

# Flags & Compiler
CXX = g++
CXXFLAGS = -Wall -O2 -MMD -MP $(INC)

# Target file name
TARGET = $(BIN_DIR)/programa${EXT}

# Default Rule (> make)
all: $(TARGET)

# Executable from .o files, copies .dll file
$(TARGET): $(OBJS)
	$(MKDIR) $(BIN_DIR)
	$(CXX) -mwindows -o $@ $^ $(LIBS)
	cp $(SDL_PATH)/bin/SDL2.dll $(BIN_DIR)

# .o from .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Headers
-include *.d

# Run Rule (> make run)
run: $(TARGET)
	./$(TARGET)

# Clean Rule (> make clean)
clean:
	$(RM) $(BUILD_DIR)/*.o
	$(RM) $(TARGET)