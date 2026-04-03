//
// Created by mynorm50 on 16/3/26.
//

#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <chrono>
#include <iostream>
#include <string>

class Benchmark {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> inicio;
    std::string nombreTarea;

public:
    void iniciar(std::string tarea) {
        nombreTarea = tarea;
        inicio = std::chrono::high_resolution_clock::now();
    }

    void finalizar() {
        auto fin = std::chrono::high_resolution_clock::now();
        auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();

        std::cout << "[BENCHMARK] " << nombreTarea << ": "
            << duracion << " microsegundos ("
            << duracion / 1000.0 << " ms)" << std::endl;
    }
};

#endif