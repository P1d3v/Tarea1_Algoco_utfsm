CXX = g++
CXXFLAGS ?= -std=c++17 -O2
TARGET = sorting.exe
INPUT_DIR ?= data/array_input
OUTPUT_DIR ?= data/array_output
MEASUREMENTS ?= data/measurements/a.csv

SOURCES = sorting.cpp algorithms/mergesort.cpp algorithms/quicksort.cpp \
          algorithms/patiencesort.cpp algorithms/sort.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -I algorithms -o $(TARGET)

run: $(TARGET)
	$(TARGET) "$(INPUT_DIR)" "$(OUTPUT_DIR)" "$(MEASUREMENTS)"

generar-inputs:
	python scripts/array_generator.py

generar_plots:
	python scripts/plot_generator.py

experimento: generar-inputs
	$(MAKE) run INPUT_DIR="$(INPUT_DIR)" OUTPUT_DIR="$(OUTPUT_DIR)" MEASUREMENTS="$(MEASUREMENTS)"

clean:
	-del /Q $(TARGET) 2>nul
