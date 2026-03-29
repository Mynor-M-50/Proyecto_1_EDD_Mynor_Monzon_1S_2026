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
    CSVLoader(Logger* logger) : logger(logger){}

    bool cargarArchivo(const std::string& ruta, void(*callback)(const Producto&)) {
        std::ifstream archivo(ruta);

        if (!archivo.is_open()) {
            logger->error("No pudo abrirse el archivo " + ruta);
            return false;
        }

        std::string linea;
        int lineaNum = 0;

        //Salta encabezado
        std::getline(archivo, linea);

        while (std::getline(archivo, linea)) {
            lineaNum++;

            std::stringstream ss(linea);
            std::string codigo, nombre, precioStr;

            if (!std::getline(ss, codigo, ',')) continue;
            if (!std::getline(ss, nombre, ',')) continue;
            if (!std::getline(ss, precioStr, ',')) continue;

            try {
                double precio = std::stod(precioStr);

                Producto p;
                p.codigoBarras = codigo;
                p.nombre = nombre;
                p.precio = precio;

                //aqui mando al catalogo
                callback(p);

            } catch (...) {
                logger->error("Error en la linea " + std::to_string(lineaNum));
            }
        }

        logger->info("El archivo fue cargado correctamente.");
        return true;
    }
};

#endif