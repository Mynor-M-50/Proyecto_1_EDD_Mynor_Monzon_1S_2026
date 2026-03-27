//
// Created by mynorm50 on 16/3/26.
//

#include "Catalogo.h"
#include <iostream>

Catalogo::Catalogo() {}

bool Catalogo::agregarProducto(const Producto& p) {

    // Validación: evitar duplicados usando Hash
    if (hash.buscar(p.codigoBarras) != nullptr) {
        std::cout << "[ERROR] Producto duplicado: " << p.codigoBarras << std::endl;
        return false;
    }

    // ✅ Insertar en todas las estructuras
    lista.insertarAlFinal(p);
    listaOrdenada.insertarOrdenadoPorNombre(p);
    avl.insertar(p);
    arbolB.insertar(p);
    arbolBPlus.insertar(p);
    hash.insertar(p);

    return true;
}

Producto* Catalogo::buscarPorCodigo(std::string codigo) {
    return hash.buscar(codigo);
}

void Catalogo::imprimirResumen() {
    std::cout << "\n--- RESUMEN CATALOGO ---\n";
    std::cout << "Total en lista: " << lista.getSize() << std::endl;
}