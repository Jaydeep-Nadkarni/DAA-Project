# ======================================================================================
# MAKEFILE: Mumbai Local Transportation Management System
# ======================================================================================
# Usage:
#   make         - Compile the project
#   make clean   - Remove compiled files
#   make run     - Compile and run the project
#   make rebuild - Clean and recompile
# ======================================================================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -I include
LDFLAGS = 
OPTIMIZATION = -O2

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = .

# Target executable
TARGET = $(BIN_DIR)/commute

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Header files (for dependency tracking)
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Default target
all: $(TARGET)

# Create executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	@echo "Linking executable: $(TARGET)"
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "Build complete! Run with: ./commute"

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	@echo "Compiling: $<"
	$(CXX) $(CXXFLAGS) $(OPTIMIZATION) -c $< -o $@

# Create directories if they don't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Clean complete!"

# Rebuild from scratch
rebuild: clean all

# Run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

# Debug build (with debugging symbols, no optimization)
debug: CXXFLAGS += -g -DDEBUG
debug: OPTIMIZATION = -O0
debug: clean $(TARGET)
	@echo "Debug build complete!"

# Help message
help:
	@echo "Mumbai Local Transportation Management System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Compile the project (default)"
	@echo "  make clean    - Remove all compiled files"
	@echo "  make rebuild  - Clean and recompile"
	@echo "  make run      - Compile and run the program"
	@echo "  make debug    - Build with debugging symbols"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Compilation command:"
	@echo "  $(CXX) $(CXXFLAGS) $(OPTIMIZATION) -o $(TARGET) $(SOURCES)"

# Phony targets (not actual files)
.PHONY: all clean rebuild run debug help

# Dependency tracking
-include $(OBJECTS:.o=.d)

# Generate dependency files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -MM -MT $(OBJ_DIR)/$*.o $< > $@
