CXX = g++
CXXFLAGS ?= -std=c++17 -O2
TARGET = matrix_multiplication.exe
INPUT_DIR ?= data/matrix_input
OUTPUT_DIR ?= data/matrix_output
MEASUREMENTS ?= data/measurements/a.csv

SOURCES = matrix_multiplication.cpp algorithms/naive.cpp algorithms/strassen.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -I algorithms -o $(TARGET)

run: $(TARGET)
	$(TARGET) "$(INPUT_DIR)" "$(OUTPUT_DIR)" "$(MEASUREMENTS)"

generate-inputs:
	python scripts/matrix_generator.py

generar_plots:
	python scripts/plot_generator.py

experiment: generate-inputs
	$(MAKE) run INPUT_DIR="$(INPUT_DIR)" OUTPUT_DIR="$(OUTPUT_DIR)" MEASUREMENTS="$(MEASUREMENTS)"

clean:
	-del /Q $(TARGET) 2>nul
