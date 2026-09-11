#include <chrono>
#include <atomic>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <sys/resource.h>
#endif

#include "algorithms/naive.h"
#include "algorithms/strassen.h"

using namespace std;
namespace fs = filesystem;
using MatrixFunction = function<Matrix(const Matrix&, const Matrix&)>;

long long medirTiempo(const MatrixFunction& algoritmo, const Matrix& matrizA, const Matrix& matrizB) {
    auto inicio = chrono::steady_clock::now();
    Matrix resultado = algoritmo(matrizA, matrizB);
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

size_t medirMemoria(const MatrixFunction& algoritmo, const Matrix& matrizA, const Matrix& matrizB) {
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
    Matrix resultado = algoritmo(matrizA, matrizB);
    size_t memoriaFinal = obtenerMemoriaProceso();
    medir = false;
    monitor.join();
    size_t memoriaMaximaRegistrada = memoriaMaxima.load();
    if (memoriaFinal > memoriaMaximaRegistrada) memoriaMaximaRegistrada = memoriaFinal;
    return memoriaMaximaRegistrada > memoriaInicial ? memoriaMaximaRegistrada - memoriaInicial : 0;
}

Matrix leerMatriz(const string& nombreArchivo) {
    Matrix matriz;
    ifstream archivo(nombreArchivo);
    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;
        vector<int> fila;
        int numero;
        istringstream entrada(linea);
        while (entrada >> numero) fila.push_back(numero);
        if (!fila.empty()) matriz.push_back(fila);
    }
    return matriz;
}

void guardarMatriz(const Matrix& matriz, const fs::path& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    for (const vector<int>& fila : matriz) {
        for (size_t j = 0; j < fila.size(); ++j) {
            if (j > 0) archivo << ' ';
            archivo << fila[j];
        }
        archivo << '\n';
    }
}

string obtenerCaracteristica(const string& nombreArchivo) {
    if (nombreArchivo.find("dispersa") != string::npos) return "dispersa";
    if (nombreArchivo.find("diagonal") != string::npos) return "diagonal";
    if (nombreArchivo.find("densa") != string::npos) return "densa";
    return "no especificada";
}

string obtenerNombreSegundaMatriz(const string& nombreArchivo) {
    string nombre = nombreArchivo;
    size_t posicion = nombre.rfind("_1.txt");
    if (posicion != string::npos) nombre.replace(posicion, 6, "_2.txt");
    return nombre;
}

int main(int argc, char* argv[]) {
    string carpetaEntrada = argc > 1 ? argv[1] : "data/matrix_input";
    string carpetaSalida = argc > 2 ? argv[2] : "data/matrix_output";
    string archivoMediciones = argc > 3 ? argv[3] : "data/measurements/a.csv";

    vector<string> nombresAlgoritmos = {"naive", "strassen"};
    vector<MatrixFunction> algoritmos = {naiveMultiply, strassenMultiply};

    fs::create_directories(carpetaEntrada);
    fs::create_directories(carpetaSalida);
    fs::path rutaMediciones(archivoMediciones);
    if (!rutaMediciones.parent_path().empty()) fs::create_directories(rutaMediciones.parent_path());
    ofstream salida(archivoMediciones);
    if (!salida.is_open()) {
        cerr << "No se pudo abrir el archivo de mediciones." << endl;
        return 1;
    }
    salida << "Archivo de entrada;Archivo de salida;Nombre del algoritmo;Tamano de la matriz;"
           << "Caracteristica de la matriz;Tiempo;Memoria usada\n";

    for (const fs::directory_entry& archivo : fs::directory_iterator(carpetaEntrada)) {
        string nombreArchivo = archivo.path().filename().string();
        if (!archivo.is_regular_file() || archivo.path().extension() != ".txt" ||
            nombreArchivo.find("_1.txt") == string::npos) continue;

        string nombreSegundoArchivo = obtenerNombreSegundaMatriz(nombreArchivo);
        fs::path rutaSegundaMatriz = fs::path(carpetaEntrada) / nombreSegundoArchivo;
        if (!fs::exists(rutaSegundaMatriz)) continue;

        Matrix matrizA = leerMatriz(archivo.path().string());
        Matrix matrizB = leerMatriz(rutaSegundaMatriz.string());
        if (matrizA.empty() || matrizB.empty()) continue;

        for (size_t i = 0; i < algoritmos.size(); ++i) {
            long long tiempo = medirTiempo(algoritmos[i], matrizA, matrizB);
            size_t memoria = medirMemoria(algoritmos[i], matrizA, matrizB);
            Matrix resultado = algoritmos[i](matrizA, matrizB);

            string nombreBase = archivo.path().stem().string();
            nombreBase.erase(nombreBase.size() - 2);
            fs::path nombreSalida = nombreBase + "_" + nombresAlgoritmos[i] + "_out.txt";
            guardarMatriz(resultado, fs::path(carpetaSalida) / nombreSalida);

                 salida << nombreArchivo << ';'
                     << nombreSalida.string() << ';'
                     << nombresAlgoritmos[i] << ';'
                     << matrizA.size() << 'x' << matrizA[0].size() << ';'
                     << obtenerCaracteristica(nombreArchivo) << ';'
                     << tiempo << ';'
                     << memoria << '\n';
            cout << nombresAlgoritmos[i] << " - " << nombreArchivo << endl;
        }
    }
    return 0;
}
