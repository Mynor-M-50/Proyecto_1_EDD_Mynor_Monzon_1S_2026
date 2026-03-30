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

public:
    CSVLoader(Logger* logger) : logger(logger) {}

    bool cargarArchivo(const std::string& ruta, void(*callback)(const Producto&)) {
        std::ifstream archivo(ruta);

        if (!archivo.is_open()) {
            logger->error("No pudo abrirse el archivo: " + ruta);
            return false;
        }

        std::string linea;
        int lineaNum = 0;

        // salta encabezado
        std::getline(archivo, linea);

        while (std::getline(archivo, linea)) {
            lineaNum++;

            std::stringstream ss(linea);
            std::string codigoBarras, nombre, categoria, fechaVencimiento, marca, precioStr, stockStr;

            if (!std::getline(ss, codigoBarras,      ',')) continue;
            if (!std::getline(ss, nombre,             ',')) continue;
            if (!std::getline(ss, categoria,          ',')) continue;
            if (!std::getline(ss, fechaVencimiento,   ',')) continue;
            if (!std::getline(ss, marca,              ',')) continue;
            if (!std::getline(ss, precioStr,          ',')) continue;
            if (!std::getline(ss, stockStr,           ',')) continue;

            try {
                Producto p;
                p.codigoBarras    = codigoBarras;
                p.nombre          = nombre;
                p.categoria       = categoria;
                p.fechaVencimiento = fechaVencimiento;
                p.marca           = marca;
                p.precio          = std::stod(precioStr);
                p.stock           = std::stoi(stockStr);

                // Validar campos vacios
                if (p.codigoBarras.empty() || p.nombre.empty() || p.categoria.empty() ||
                    p.fechaVencimiento.empty() || p.marca.empty()) {
                    logger->error("Linea " + std::to_string(lineaNum) + ": campo vacio detectado");
                    continue;
                    }

                // Validar precio positivo
                if (p.precio <= 0) {
                    logger->error("Linea " + std::to_string(lineaNum) + ": precio invalido (" + precioStr + ")");
                    continue;
                }

                // Validar stock no negativo
                if (p.stock < 0) {
                    logger->error("Linea " + std::to_string(lineaNum) + ": stock invalido (" + stockStr + ")");
                    continue;
                }

                // Validar formato de fecha (YYYY-MM-DD)
                if (p.fechaVencimiento.size() != 10 ||
                    p.fechaVencimiento[4] != '-' || p.fechaVencimiento[7] != '-') {
                    logger->error("Linea " + std::to_string(lineaNum) + ": fecha invalida (" + p.fechaVencimiento + ")");
                    continue;
                    }

                callback(p);

            } catch (...) {
                logger->error("Error en linea " + std::to_string(lineaNum) + ": " + linea);
            }
        }

        logger->info("Archivo cargado correctamente: " + ruta);
        return true;
    }
};

#endif