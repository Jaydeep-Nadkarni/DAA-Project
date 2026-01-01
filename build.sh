#!/bin/bash
# ======================================================================================
# BUILD SCRIPT: Mumbai Local Transportation Management System (Linux/macOS)
# ======================================================================================
# This script compiles the project on Linux/macOS using g++
# Usage: ./build.sh [options]
#   Options:
#     clean   - Remove all compiled files
#     debug   - Build with debugging symbols
#     run     - Build and run the program
# ======================================================================================

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
CXX=g++
CXXFLAGS="-std=c++11 -Wall -Wextra -I include"
OPTIMIZATION="-O2"
TARGET="commute"
OBJ_DIR="obj"

# Function to print colored messages
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Function to clean build artifacts
clean_build() {
    print_info "Cleaning build artifacts..."
    rm -rf $OBJ_DIR $TARGET
    print_success "Clean complete!"
}

# Function to check compiler
check_compiler() {
    if ! command -v $CXX &> /dev/null; then
        print_error "$CXX compiler not found!"
        print_info "Please install g++ using your package manager:"
        print_info "  Ubuntu/Debian: sudo apt-get install g++"
        print_info "  Fedora/RHEL:   sudo dnf install gcc-c++"
        print_info "  macOS:         xcode-select --install"
        exit 1
    fi
    print_info "Compiler found: $CXX"
    $CXX --version | head -n 1
}

# Function to compile source files
compile_sources() {
    print_info "Creating object directory..."
    mkdir -p $OBJ_DIR
    
    print_info "Compiling source files..."
    
    local sources=(
        "main"
        "station"
        "graph"
        "ticketing"
        "scheduling"
        "queue_manager"
        "analytics"
    )
    
    for src in "${sources[@]}"; do
        print_info "  Compiling src/${src}.cpp..."
        $CXX $CXXFLAGS $OPTIMIZATION -c src/${src}.cpp -o $OBJ_DIR/${src}.o
    done
    
    print_success "All source files compiled successfully!"
}

# Function to link executable
link_executable() {
    print_info "Linking executable..."
    $CXX $OBJ_DIR/*.o -o $TARGET
    print_success "Executable created: $TARGET"
}

# Function to build project
build() {
    echo ""
    echo "========================================================================"
    echo "   MUMBAI LOCAL TRANSPORTATION MANAGEMENT SYSTEM - BUILD SCRIPT"
    echo "========================================================================"
    echo ""
    
    check_compiler
    compile_sources
    link_executable
    
    echo ""
    echo "========================================================================"
    echo "   BUILD SUCCESSFUL!"
    echo "========================================================================"
    echo ""
    print_success "To run the program, type: ./$TARGET"
    echo ""
}

# Function to build in debug mode
build_debug() {
    print_info "Building in DEBUG mode..."
    OPTIMIZATION="-O0 -g -DDEBUG"
    clean_build
    build
    print_success "Debug build complete!"
}

# Function to run the program
run_program() {
    if [ ! -f "$TARGET" ]; then
        print_warning "Executable not found. Building first..."
        build
    fi
    
    print_info "Running $TARGET..."
    echo ""
    ./$TARGET
}

# Main script logic
case "${1:-build}" in
    clean)
        clean_build
        ;;
    debug)
        build_debug
        ;;
    run)
        build
        run_program
        ;;
    rebuild)
        clean_build
        build
        ;;
    help)
        echo "Mumbai Local Transportation Management System - Build Script"
        echo ""
        echo "Usage: ./build.sh [option]"
        echo ""
        echo "Options:"
        echo "  (none)   - Build the project (default)"
        echo "  clean    - Remove all compiled files"
        echo "  debug    - Build with debugging symbols"
        echo "  run      - Build and run the program"
        echo "  rebuild  - Clean and rebuild"
        echo "  help     - Show this help message"
        echo ""
        ;;
    *)
        build
        ;;
esac
