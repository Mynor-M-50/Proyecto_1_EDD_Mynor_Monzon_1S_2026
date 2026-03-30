//
// Created by mynorm50 on 26/3/26.
//

#include <iostream>
#include <string>
#include "Catalogo/Catalogo.h"
#include "Carga/CSVLoader.h"
#include "Utils/Logger.h"
#include "Graficos/ReporteGraficos.h"
#include "Utils/Benchmark.h"

Catalogo catalogo;
Logger logger;
CSVLoader loader(&logger);
Benchmark timer;
ReporteGraficos rep;
bool csvCargado = false;

void insertarEnCatalogo(const Producto& p) {
    catalogo.agregarProducto(p);
}

// Funcion auxiliar para leer numeros de forma segura
int leerEntero() {
    std::string linea;
    std::getline(std::cin, linea);
    try {
        return std::stoi(linea);
    } catch (...) {
        return -1;
    }
}

void menuBusqueda() {
    std::cout << "\n--- Busqueda ---\n";
    std::cout << "1. Por Codigo de Barras (Hash)\n";
    std::cout << "2. Por Nombre (AVL)\n";
    std::cout << "3. Por Categoria (Arbol B+)\n";
    std::cout << "4. Por Rango de Fecha (Arbol B)\n";
    std::cout << "Seleccione: ";
    int op = leerEntero();

    std::string input;
    switch (op) {
        case 1:
            std::cout << "Ingrese codigo de barras: ";
            std::getline(std::cin, input);
            timer.iniciar("Busqueda por codigo");
            {
                Producto* p = catalogo.buscarPorCodigo(input);
                timer.finalizar();
                if (p) std::cout << "Encontrado: " << p->nombre << " | $" << p->precio << "\n";
                else std::cout << "[INFO] No encontrado.\n";
            }
            break;
        case 2:
            std::cout << "Ingrese nombre: ";
            std::getline(std::cin, input);
            timer.iniciar("Busqueda por nombre");
            {
                Producto* p = catalogo.buscarPorNombre(input);
                timer.finalizar();
                if (p) std::cout << "Encontrado: " << p->nombre << " | " << p->categoria << "\n";
                else std::cout << "[INFO] No encontrado.\n";
            }
            break;
        case 3:
            std::cout << "Ingrese categoria: ";
            std::getline(std::cin, input);
            timer.iniciar("Busqueda por categoria");
            catalogo.buscarPorCategoria(input);
            timer.finalizar();
            break;
        case 4:
            {
                std::string desde, hasta;
                std::cout << "Ingrese fecha inicio (YYYY-MM-DD): ";
                std::getline(std::cin, desde);
                std::cout << "Ingrese fecha fin   (YYYY-MM-DD): ";
                std::getline(std::cin, hasta);
                timer.iniciar("Busqueda por rango");
                catalogo.buscarPorRango(desde, hasta);
                timer.finalizar();
            }
            break;
        default:
            std::cout << "[ERROR] Opcion invalida.\n";
    }
}

int main() {
    int opcion;
    do {
        std::cout << "\n========== MENU PRINCIPAL ==========\n";
        std::cout << "1. Cargar Archivo CSV\n";
        std::cout << "2. Buscar Producto\n";
        std::cout << "3. Eliminar Producto\n";
        std::cout << "4. Deshacer (Rollback)\n";
        std::cout << "5. Reportes Graphviz\n";
        std::cout << "6. Resumen\n";
        std::cout << "0. Salir\n";
        std::cout << "Seleccione: ";

        opcion = leerEntero();

        switch (opcion) {
            case 1: {
                std::cout << "Ruta CSV: ";
                std::string ruta;
                std::getline(std::cin, ruta);
                timer.iniciar("Carga");
                loader.cargarArchivo(ruta, insertarEnCatalogo);
                timer.finalizar();
                csvCargado = true;
                break;
            }
            case 2: if(csvCargado) menuBusqueda(); else std::cout << "Cargue CSV primero.\n"; break;
            case 3: {
                std::cout << "Codigo a eliminar: ";
                std::string cod;
                std::getline(std::cin, cod);
                catalogo.eliminarProducto(cod);
                break;
            }
            case 4: catalogo.rollback(); break;
            case 5: {
                rep.generarAVL(catalogo.getAVL(), "avl_reporte");
                rep.generarArbolB(catalogo.getArbolB(), "b_reporte");
                rep.generarArbolBPlus(catalogo.getArbolBPlus(), "bplus_reporte");
                rep.generarTablaHash(catalogo.getTablaHash(), "hash_reporte");
                std::cout << "Reportes generados.\n";
                break;
            }
            case 6: catalogo.imprimirResumen(); break;
            case 0: std::cout << "Adios!\n"; break;
            default: std::cout << "Opcion invalida.\n"; break;
        }
    } while (opcion != 0);
    return 0;
}
