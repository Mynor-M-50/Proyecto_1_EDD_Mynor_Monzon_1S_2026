//
// Created by mynorm50 on 16/3/26.
//

#include "Catalogo.h"
#include <iostream>

Catalogo::Catalogo() {}

bool Catalogo::agregarProducto(const Producto& p) {

    // Validacin: evitar duplicados usando hash
    if (hash.buscar(p.codigoBarras) != nullptr) {
        std::cout << "[DUPLICADO] " << p.codigoBarras << " - " << p.nombre << "\n";
        return false;
    }

    // Insertar en orden, con rollback si algo falla
    lista.insertarAlFinal(p);
    listaOrdenada.insertarOrdenadoPorNombre(p);

    if (!avl.insertar(p)) {
        lista.eliminarPorCodigo(p.codigoBarras);
        listaOrdenada.eliminarPorCodigo(p.codigoBarras);
        std::cout << "[ERROR] Fallo AVL, rollback aplicado: " << p.nombre << "\n";
        return false;
    }

    arbolB.insertar(p);
    arbolBPlus.insertar(p);
    hash.insertar(p);

    return true;
}

void Catalogo::eliminarProducto(const std::string& codigo) {
    Producto* p = hash.buscar(codigo);
    if (p == nullptr) {
        std::cout << "[ERROR] Producto no encontrado: " << codigo << "\n";
        return;
    }

    // Guardar copia en la pila antes de eliminar
    historialEliminados.push(*p);

    std::string nombre    = p->nombre;
    std::string fecha     = p->fechaVencimiento;
    std::string categoria = p->categoria;

    avl.eliminar(nombre);
    arbolB.eliminar(fecha);
    arbolBPlus.eliminar(categoria, codigo);
    lista.eliminarPorCodigo(codigo);
    hash.eliminar(codigo);

    std::cout << "[INFO] Producto eliminado y guardado en historial: " << nombre << " [" << codigo << "]\n";
}

void Catalogo::rollback() {
    if (historialEliminados.estaVacia()) {
        std::cout << "[ROLLBACK] No hay eliminaciones para deshacer.\n";
        return;
    }

    Producto p = historialEliminados.pop();
    std::cout << "[ROLLBACK] Restaurando: " << p.nombre << " [" << p.codigoBarras << "]\n";
    agregarProducto(p);
    std::cout << "[ROLLBACK] Producto restaurado exitosamente.\n";
}

Producto* Catalogo::buscarPorCodigo(std::string codigo) {
    return hash.buscar(codigo);
}

Producto* Catalogo::buscarPorNombre(const std::string& nombre) {
    return avl.buscar(nombre);
}

void Catalogo::buscarPorCategoria(const std::string& categoria) {
    arbolBPlus.buscarPorCategoria(categoria);
}

void Catalogo::buscarPorRango(const std::string& desde, const std::string& hasta) {
    arbolB.buscarPorRango(desde, hasta);
}

void Catalogo::imprimirResumen() {
    std::cout << "\n--- Resumen Catalogo ---\n";
    std::cout << "Total productos: " << lista.getSize() << "\n";
}