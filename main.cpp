//
// Created by mynorm50 on 26/3/26.
//

#include <iostream>
#include "Catalogo/Catalogo.h"
#include "Carga/CSVLoader.h"
#include "Utils/Logger.h"
#include "Graficos/ReporteGraficos.h"

Catalogo catalogo;

void insertarEnCatalogo(const Producto& p) {
    catalogo.agregarProducto(p);
}

int main() {

    Logger logger;
    CSVLoader loader(&logger);

    std::cout << "\n--- Carga Masiva ---\n";

    loader.cargarArchivo("productos.csv", insertarEnCatalogo);

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

    return 0;
}
