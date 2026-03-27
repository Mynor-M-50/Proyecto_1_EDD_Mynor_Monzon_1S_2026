//
// Created by mynorm50 on 16/3/26.
//

#ifndef PROYECTO_1_LOGGER_H
#define PROYECTO_1_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

class Logger {
private:
    std::ofstream archivo;

public:
    Logger(const std::string& nombreArchivo = "log.txt") {
        archivo.open(nombreArchivo, std::ios::app);;
    }

    ~Logger() {
        if (archivo.is_open()) {
            archivo.close();
        }
    }

    void info(const std::string& mensaje) {
        std::cout << "[INFO]" << mensaje << std::endl;
        if (archivo.is_open()) {
            archivo << "[INFO]" << mensaje << std::endl;
        }
    }

    void error(const std::string& mensaje) {
        std::cout << "[ERROR]" << mensaje << std::endl;
        if (archivo.is_open()) {
            archivo << "[ERROR]" << mensaje << std::endl;
        }
    }
};

#endif //PROYECTO_1_LOGGER_H