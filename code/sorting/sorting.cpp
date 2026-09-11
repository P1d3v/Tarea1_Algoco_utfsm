#include <chrono>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

#include "algorithms/mergesort.h"
#include "algorithms/patiencesort.h"
#include "algorithms/quicksort.h"
#include "algorithms/sort.h"

using namespace std;
namespace fs = filesystem;
using SortFunction = function<void(vector<int>&)>;

long long medirTiempo(const SortFunction& algoritmo, const vector<int>& entrada) {
    vector<int> datos(entrada);
    auto inicio = chrono::steady_clock::now();
    algoritmo(datos);
    auto termino = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(termino - inicio).count();
}

size_t obtenerMemoriaProceso() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS memoria;
    memoria.cb = sizeof(PROCESS_MEMORY_COUNTERS);
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memoria, sizeof(memoria))) {
        return static_cast<size_t>(memoria.WorkingSetSize);
    }
    return 0;
#else
    struct rusage uso;
    getrusage(RUSAGE_SELF, &uso);
    return static_cast<size_t>(uso.ru_maxrss) * 1024;
#endif
}

size_t medirMemoria(const SortFunction& algoritmo, const vector<int>& entrada) {
    vector<int> datos(entrada);
    const size_t memoriaInicial = obtenerMemoriaProceso();
    atomic<bool> medir = true;
    atomic<size_t> memoriaMaxima = memoriaInicial;
    thread monitor([&]() {
        while (medir) {
            size_t memoriaActual = obtenerMemoriaProceso();
            size_t memoriaRegistrada = memoriaMaxima.load();
            while (memoriaActual > memoriaRegistrada &&
                   !memoriaMaxima.compare_exchange_weak(memoriaRegistrada, memoriaActual)) {
            }
        }
    });
    algoritmo(datos);
    size_t memoriaFinal = obtenerMemoriaProceso();
    medir = false;
    monitor.join();
    size_t memoriaMaximaRegistrada = memoriaMaxima.load();
    if (memoriaFinal > memoriaMaximaRegistrada) memoriaMaximaRegistrada = memoriaFinal;
    return memoriaMaximaRegistrada > memoriaInicial ? memoriaMaximaRegistrada - memoriaInicial : 0;
}

vector<int> leerArreglo(const string& nombreArchivo) {
    vector<int> arreglo;
    ifstream archivo(nombreArchivo);
    int numero;
    while (archivo >> numero) arreglo.push_back(numero);
    return arreglo;
}

void guardarArreglo(const vector<int>& arreglo, const fs::path& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    for (size_t i = 0; i < arreglo.size(); ++i) {
        if (i > 0) archivo << ' ';
        archivo << arreglo[i];
    }
    archivo << '\n';
}

bool estaOrdenado(const vector<int>& arreglo) {
    for (size_t i = 1; i < arreglo.size(); ++i) {
        if (arreglo[i - 1] > arreglo[i]) return false;
    }
    return true;
}

string obtenerCaracteristica(const string& nombreArchivo) {
    if (nombreArchivo.find("ascendente") != string::npos) return "ordenado";
    if (nombreArchivo.find("descendente") != string::npos) return "decrecientemente";
    if (nombreArchivo.find("aleatorio") != string::npos) return "aleatorio";
    return "no especificada";
}

int main(int argc, char* argv[]) {
    string carpetaEntrada = argc > 1 ? argv[1] : "data/array_input";
    string carpetaSalida = argc > 2 ? argv[2] : "data/array_output";
    string archivoMediciones = argc > 3 ? argv[3] : "data/measurements/a.csv";

    vector<string> nombresAlgoritmos = {"merge_sort", "quick_sort", "patience_sort", "std_sort"};
    vector<SortFunction> algoritmos = {
        mergeSort,
        quickSort,
        patienceSort,
        [](vector<int>& arreglo) { sortArray(arreglo); }
    };

    fs::create_directories(carpetaEntrada);
    fs::create_directories(carpetaSalida);
    fs::path rutaMediciones(archivoMediciones);
    if (!rutaMediciones.parent_path().empty()) fs::create_directories(rutaMediciones.parent_path());
    ofstream salida(archivoMediciones);
    if (!salida.is_open()) {
        cerr << "No se pudo abrir el archivo de mediciones." << endl;
        return 1;
    }
    salida << "Archivo de entrada;Archivo de salida;Nombre del algoritmo;Tamano del arreglo;"
           << "Caracteristica del arreglo;Resultado correcto;Tiempo;Memoria usada\n";

    for (const fs::directory_entry& archivo : fs::directory_iterator(carpetaEntrada)) {
        if (!archivo.is_regular_file() || archivo.path().extension() != ".txt") continue;
        vector<int> arreglo = leerArreglo(archivo.path().string());
        if (arreglo.empty()) continue;

        for (size_t i = 0; i < algoritmos.size(); ++i) {
            long long tiempo = medirTiempo(algoritmos[i], arreglo);
            size_t memoria = medirMemoria(algoritmos[i], arreglo);
            vector<int> resultado = arreglo;
            algoritmos[i](resultado);

            fs::path nombreSalida = archivo.path().stem().string() + "_" +
                                    nombresAlgoritmos[i] + "_out.txt";
            guardarArreglo(resultado, fs::path(carpetaSalida) / nombreSalida);

                 salida << archivo.path().filename().string() << ';'
                     << nombreSalida.string() << ';'
                     << nombresAlgoritmos[i] << ';'
                     << arreglo.size() << ';'
                     << obtenerCaracteristica(archivo.path().filename().string()) << ';'
                     << (estaOrdenado(resultado) ? "si" : "no") << ';'
                     << tiempo << ';'
                     << memoria << '\n';
            cout << nombresAlgoritmos[i] << " - " << archivo.path().filename().string() << endl;
        }
    }
    return 0;
}
