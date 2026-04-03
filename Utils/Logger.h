#ifndef PROYECTO_1_LOGGER_H
#define PROYECTO_1_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

class Logger {
private:
    std::ofstream archivo;
    int conteoErroresFormato = 0;
    int conteoErroresDatos   = 0;
    int conteoDuplicados     = 0;

public:
    Logger(const std::string& nombreArchivo = "log.txt") {
        archivo.open(nombreArchivo, std::ios::trunc);
    }

    ~Logger() {
        if (archivo.is_open()) archivo.close();
    }

    void info(const std::string& mensaje) {
        std::cout << "[INFO] " << mensaje << std::endl;
        if (archivo.is_open()) archivo << "[INFO] " << mensaje << std::endl;
    }

    void error(const std::string& mensaje) {
        // Solo va al archivo, NO a consola
        if (archivo.is_open()) archivo << "[ERROR] " << mensaje << std::endl;
        conteoErroresDatos++;
    }

    void errorFormato(const std::string& mensaje) {
        if (archivo.is_open()) archivo << "[FORMATO] " << mensaje << std::endl;
        conteoErroresFormato++;
    }

    void errorDuplicado(const std::string& mensaje) {
        if (archivo.is_open()) archivo << "[DUPLICADO] " << mensaje << std::endl;
        conteoDuplicados++;
    }

    void imprimirResumenCarga(int cargados) {
        std::cout << "\n[CARGA] Productos cargados  : " << cargados << "\n";
        std::cout << "[CARGA] Errores de formato  : " << conteoErroresFormato << "\n";
        std::cout << "[CARGA] Datos invalidos     : " << conteoErroresDatos << "\n";
        std::cout << "[CARGA] Duplicados          : " << conteoDuplicados << "\n";
        std::cout << "[CARGA] Total procesado     : "
            << (cargados + conteoErroresFormato + conteoErroresDatos + conteoDuplicados)
            << "\n";
        std::cout << "[CARGA] Detalle completo en : log.txt\n\n";
    }

    void resetContadores() {
        conteoErroresFormato = 0;
        conteoErroresDatos   = 0;
        conteoDuplicados     = 0;
    }
};

#endif