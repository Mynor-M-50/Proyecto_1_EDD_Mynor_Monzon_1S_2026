//
// Created by mynorm50 on 7/3/26.
//

#include <iostream>
#include "Modelo/Producto.h"
#include "Estructuras/ListaEnlazada.h"
#include "Estructuras/ArbolAVL.h"

int main() {
    // 1. Crear algunos productos de prueba
    Producto p1("Leche", "101", "Lacteos", "2026-05-01", "Dos Pinos", 15.50, 100);
    Producto p2("Arroz", "102", "Granos", "2027-01-15", "Ideal", 8.00, 50);
    Producto p3("Zanahoria", "103", "Verduras", "2026-04-10", "Campo", 3.25, 200);
    Producto p4("Avena", "104", "Cereales", "2026-12-20", "Quaker", 12.00, 80);

    // 2. Probar Listas
    ListaEnlazada<Producto> listaNoOrdenada;
    ListaEnlazada<Producto> listaOrdenada;

    listaNoOrdenada.insertarAlFinal(p1);
    listaNoOrdenada.insertarAlFinal(p2);
    listaNoOrdenada.insertarAlFinal(p3);
    listaNoOrdenada.insertarAlFinal(p4);

    listaOrdenada.insertarOrdenadoPorNombre(p1);
    listaOrdenada.insertarOrdenadoPorNombre(p2);
    listaOrdenada.insertarOrdenadoPorNombre(p3);
    listaOrdenada.insertarOrdenadoPorNombre(p4);

    std::cout << "--- LISTA NO ORDENADA (Orden de insercion) ---\n";
    listaNoOrdenada.imprimirNombres();

    std::cout << "\n--- LISTA ORDENADA (Por nombre) ---\n";
    listaOrdenada.imprimirNombres();

    // 3. Probar AVL
    ArbolAVL arbol;
    arbol.insertar(p1);
    arbol.insertar(p2);
    arbol.insertar(p3);
    arbol.insertar(p4);

    std::cout << "\n--- ARBOL AVL (Inorden - Deberia estar ordenado) ---\n";
    arbol.imprimirInorden();

    // 4. Probar Búsqueda en AVL
    std::cout << "\nBuscando 'Arroz' en AVL...\n";
    Producto* encontrado = arbol.buscar("Arroz");
    if (encontrado) {
        std::cout << "Encontrado! Precio: Q" << encontrado->precio << "\n";
    } else {
        std::cout << "No encontrado.\n";
    }

    return 0;
}