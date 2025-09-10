# Makefile for C++ project (Windows compatible)

# Compiler and flags
CXX = g++
CXXFLAGS = -fdiagnostics-color=always -g -std=c++17 -Iinclude
LDFLAGS = -Llibs -lglew32 -lgdi32 -lopengl32 -lglu32

# Find all .cpp files in the current directory
SOURCES = $(wildcard *.cpp)

# Target executable
TARGET = main.exe

# Default target - simple approach for Windows
all: $(TARGET)

# Link all cpp files directly (simple approach)
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

# Clean build files
clean:
	-del $(TARGET)

# Rebuild everything
rebuild: clean all

# Run the executable
run: $(TARGET)
	$(TARGET)

.PHONY: all clean rebuild run
