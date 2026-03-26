//
// Created by mynorm50 on 7/3/26.
//

//
// Created by mynorm50 on 7/3/26.
//

#include <iostream>
#include "Modelo/Producto.h"
#include "Estructuras/ListaEnlazada.h"
#include "Estructuras/ArbolAVL.h"
#include "Estructuras/ArbolB.h"
#include "Estructuras/ArbolBPlus.h"
#include "Estructuras/TablaHash.h"

int main() {
    // 1. Crear algunos productos de prueba
    Producto p1("Leche",    "101", "Lacteos",  "2026-05-01", "Dos Pinos",  15.50, 100);
    Producto p2("Arroz",    "102", "Granos",   "2027-01-15", "Ideal",       8.00,  50);
    Producto p3("Zanahoria","103", "Verduras", "2026-04-10", "Campo",       3.25, 200);
    Producto p4("Avena",    "104", "Cereales", "2026-12-20", "Quaker",     12.00,  80);
    Producto p5("Mango",    "105", "Frutas",   "2026-06-01", "Frutas GT",   5.00, 150);
    Producto p6("Naranja",  "106", "Frutas",   "2026-06-15", "Frutas GT",   4.00, 120);
    Producto p7("Platano",  "107", "Frutas",   "2026-07-01", "Frutas GT",   2.50, 200);

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

    // 4. Probar búsqueda en AVL
    std::cout << "\nBuscando 'Arroz' en AVL...\n";
    Producto* encontrado = arbol.buscar("Arroz");
    if (encontrado) {
        std::cout << "Encontrado! Precio: Q" << encontrado->precio << "\n";
    } else {
        std::cout << "No encontrado.\n";
    }

    // 5. Prueba de rotacion DD (cuando FE llega a 2)
    std::cout << "\n==============================\n";
    std::cout << "PRUEBA DE ROTACION EN AVL\n";
    std::cout << "==============================\n";

    ArbolAVL arbolRotacion;

    Producto r1("Aceite", "201", "Despensa", "2026-08-01", "Mazola", 18.00, 30);
    Producto r2("Batido", "202", "Bebidas", "2026-09-10", "Nestle", 6.50, 40);
    Producto r3("Cereal", "203", "Cereales", "2027-01-01", "Kelloggs", 22.00, 25);

    std::cout << "\nInsertando: " << r1.nombre << "\n";
    arbolRotacion.insertar(r1);

    std::cout << "Insertando: " << r2.nombre << "\n";
    arbolRotacion.insertar(r2);

    std::cout << "Insertando: " << r3.nombre << "\n";
    std::cout << "Aqui el arbol debe rotar porque el FE de la raiz llega a 2.\n";
    arbolRotacion.insertar(r3);

    std::cout << "\n--- ARBOL AVL DESPUES DE LA ROTACION ---\n";
    arbolRotacion.imprimirInorden();

    std::cout << "\nSi todo esta bien, el orden debe ser:\n";
    std::cout << "Aceite\nBatido\nCereal\n";
    std::cout << "Y el nodo central logico del arbol deberia ser Batido.\n";

    std::cout << "\n==============================\n";
    std::cout << "PRUEBA ARBOL B (d=2)\n";
    std::cout << "==============================\n";

    ArbolB arbolB;
    arbolB.insertar(p1); // Leche
    arbolB.insertar(p2); // Arroz
    arbolB.insertar(p3); // Zanahoria
    arbolB.insertar(p4); // Avena

    std::cout << "\nTotal productos insertados: 7\n";
    std::cout << "Buscando 'Zanahoria'...\n";
    Producto* z = arbolB.buscar("Zanahoria");
    if (z) {
        std::cout << "Zanahoria encontrada! Precio: Q" << z->precio << "\n";
    } else {
        std::cout << "Zanahoria NO encontrada - confirma el bug\n";
    }

    // Forzar un split con más productos
    arbolB.insertar(p5);
    arbolB.insertar(p6);
    arbolB.insertar(p7);

    std::cout << "\n--- ARBOL B (Inorden - debe estar ordenado) ---\n";
    arbolB.imprimirInorden();

    std::cout << "\nBuscando 'Mango' en Arbol B...\n";
    Producto* encontradoB = arbolB.buscar("Mango");
    if (encontradoB) {
        std::cout << "Encontrado! Precio: Q" << encontradoB->precio << "\n";
    } else {
        std::cout << "No encontrado.\n";
    }

    std::cout << "\n==============================\n";
    std::cout << "PRUEBA ARBOL B+ (d=2)\n";
    std::cout << "==============================\n";

    ArbolBPlus arbolBP;
    // Insertamos los mismos 7 productos
    arbolBP.insertar(p1); arbolBP.insertar(p2); arbolBP.insertar(p3);
    arbolBP.insertar(p4); arbolBP.insertar(p5); arbolBP.insertar(p6);
    arbolBP.insertar(p7);

    std::cout << "\n--- RECORRIDO LINEAL (Por hojas enlazadas) ---\n";
    arbolBP.imprimirLineal();

    std::cout << "\nBuscando 'Zanahoria' en B+...\n";
    Producto* encontradoBP = arbolBP.buscar("Zanahoria");
    if (encontradoBP) {
        std::cout << "Encontrado! Codigo: " << encontradoBP->codigoBarras << "\n";
    }

    TablaHash hash;
    hash.insertar(p1);
    Producto* h = hash.buscar("101");
    if(h) std::cout << "Hash funciona: " << h->nombre << "\n";

    return 0;
}