//
// Created by mynorm50 on 16/3/26.
//

#ifndef PROYECTO_1_CSVLOADER_H
#define PROYECTO_1_CSVLOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../Modelo/Producto.h"
#include "../Utils/Logger.h"

class CSVLoader {
private:
    Logger* logger;

    // Convierte a minúsculas (simple)
    std::string toLower(std::string s) {
        for (char& c : s) {
            if (c >= 'A' && c <= 'Z') c = c + 32;
        }
        return s;
    }

public:
    CSVLoader(Logger* logger) : logger(logger) {}

    bool cargarArchivo(const std::string& ruta, bool(*callback)(const Producto&)) {
        std::ifstream archivo(ruta);

        if (!archivo.is_open()) {
            logger->error("No pudo abrirse el archivo: " + ruta);
            return false;
        }

        std::string linea;
        int lineaNum = 0;

        // Leer encabezado
        std::getline(archivo, linea);
        std::stringstream header(linea);

        std::string h1, h2, h3, h4, h5, h6, h7;

        std::getline(header, h1, ',');
        std::getline(header, h2, ',');
        std::getline(header, h3, ',');
        std::getline(header, h4, ',');
        std::getline(header, h5, ',');
        std::getline(header, h6, ',');
        std::getline(header, h7, ',');

        h1 = toLower(h1);
        h2 = toLower(h2);

        // Detectar formato
        bool formatoPDF = (h1 == "nombre" && h2 == "codigobarra");

        while (std::getline(archivo, linea)) {
            lineaNum++;

            std::stringstream ss(linea);
            std::string f1, f2, f3, f4, f5, f6, f7;

            if (!std::getline(ss, f1, ',') ||
                !std::getline(ss, f2, ',') ||
                !std::getline(ss, f3, ',') ||
                !std::getline(ss, f4, ',') ||
                !std::getline(ss, f5, ',') ||
                !std::getline(ss, f6, ',') ||
                !std::getline(ss, f7, ',')) {
                logger->errorFormato("Linea " + std::to_string(lineaNum) + ": formato incompleto");
                continue;
            }

            try {
                Producto p;

                if (formatoPDF) {
                    // Nombre, CodigoBarra,...
                    p.nombre           = f1;
                    p.codigoBarras     = f2;
                } else {
                    // codigoBarras, nombre,...
                    p.codigoBarras     = f1;
                    p.nombre           = f2;
                }

                p.categoria        = f3;
                p.fechaVencimiento = f4;
                p.marca            = f5;
                p.precio           = std::stod(f6);
                p.stock            = std::stoi(f7);

                // Validaciones
                if (p.codigoBarras.empty() || p.nombre.empty() || p.categoria.empty() ||
                    p.fechaVencimiento.empty() || p.marca.empty()) {
                    logger->error("Linea " + std::to_string(lineaNum) + ": campo vacio detectado");
                    continue;
                }

                if (p.precio <= 0) {
                    logger->error("Linea " + std::to_string(lineaNum) + ": precio invalido (" + f6 + ")");
                    continue;
                }

                if (p.stock < 0) {
                    logger->error("Linea " + std::to_string(lineaNum) + ": stock invalido (" + f7 + ")");
                    continue;
                }

                if (p.fechaVencimiento.size() != 10 ||
                    p.fechaVencimiento[4] != '-' || p.fechaVencimiento[7] != '-') {
                    logger->error("Linea " + std::to_string(lineaNum) + ": fecha invalida (" + p.fechaVencimiento + ")");
                    continue;
                }

                if (!callback(p)) {
                    logger->errorDuplicado("Linea " + std::to_string(lineaNum) + ": duplicado - " + p.codigoBarras);
                }

            } catch (...) {
                logger->error("Error en linea " + std::to_string(lineaNum) + ": " + linea);
            }
        }

        logger->info("Archivo cargado correctamente: " + ruta);
        return true;
    }
};

#endif