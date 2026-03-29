//
// Created by mynorm50 on 26/3/26.
//

#include <iostream>
#include "Catalogo/Catalogo.h"
#include "Carga/CSVLoader.h"
#include "Utils/Logger.h"
#include "Graficos/ReporteGraficos.h"
#include "Utils/Benchmark.h"


Catalogo catalogo;

void insertarEnCatalogo(const Producto& p) {
    catalogo.agregarProducto(p);
}

int main() {

    Logger logger;
    CSVLoader loader(&logger);
    Benchmark timer;

    std::cout << "\n--- Carga Masiva y Benchmark ---\n";

    timer.iniciar("Insercion masiva en todas las estructuras");
    loader.cargarArchivo("productos.csv", insertarEnCatalogo);
    timer.finalizar();

    std::cout << "\n--- Benchmark de Busqueda ---\n";

    timer.iniciar("Busqueda por codigo (Hash)");
    catalogo.buscarPorCodigo("101");
    timer.finalizar();

    std::cout << "\n--- Busqueda ---\n";

    Producto* p = catalogo.buscarPorCodigo("101");

    if (p) {
        std::cout << "Encontrado en catalogo: " << p->nombre << std::endl;
    } else {
        std::cout << "No encontrado\n";
    }

    catalogo.imprimirResumen();

    ReporteGraficos rep;
    rep.generarAVL(catalogo.getAVL());
    rep.generarArbolB(catalogo.getArbolB());
    rep.generarArbolBPlus(catalogo.getArbolBPlus());
    rep.generarTablaHash(catalogo.getTablaHash());

    return 0;
}
