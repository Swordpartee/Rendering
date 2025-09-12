# Makefile for C++ project (Windows compatible)

# Compiler and flags
CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g -std=c++17 -Iinclude
LDFLAGS = -Llibs -lglew32 -lgdi32 -lopengl32 -lglu32

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# Find all .cpp files in src directory
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/ParteeRenderer.cpp $(SRC_DIR)/ParteeWindow.cpp

# Target executable in build directory
TARGET = $(BUILD_DIR)/main.exe

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory if it doesn't exist
$(BUILD_DIR):
	if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

# Link all cpp files from src directory
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	if exist "$(BUILD_DIR)\*.exe" del "$(BUILD_DIR)\*.exe"
	if exist "$(BUILD_DIR)\*.o" del "$(BUILD_DIR)\*.o"

# Rebuild everything
rebuild: clean all

# Run the executable
run: $(TARGET)
	$(BUILD_DIR)/main.exe

.PHONY: all clean rebuild run
