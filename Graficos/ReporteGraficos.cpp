//
// Created by mynorm50 on 7/3/26.
//

#include "ReporteGraficos.h"
#include "../Estructuras/NodoArbolAVL.h"
#include "../Estructuras/NodoArbolB.h"
#include <fstream>
#include <cstdlib>

//////////////////////////////////////////////////////////////
// AVL
//////////////////////////////////////////////////////////////

void ReporteGraficos::generarAVL(ArbolAVL& arbol) {

    system("mkdir -p Reportes"); // crea carpeta si no existe

    std::ofstream archivo("Reportes/avl.dot");

    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear avl.dot\n";
        return;
    }

    archivo << "digraph AVL {\n";
    archivo << "    node [shape=record, style=filled, fillcolor=lightblue];\n";
    archivo << "    label=\"Árbol AVL de Productos\";\n";

    generarNodosAVL(archivo, arbol.getRaiz());

    archivo << "}\n";
    archivo.close();

    std::cout << "[INFO] Generando imagen AVL...\n";
    system("dot -Tpng Reportes/avl.dot -o Reportes/avl.png");
    std::cout << "[INFO] Imagen Reportes/avl.png generada con éxito.\n";
}

void ReporteGraficos::generarNodosAVL(std::ofstream& archivo, NodoArbolAVL* nodo) {
    if (nodo == nullptr) return;

    archivo << "    node" << nodo << " [label=\"{ "
            << nodo->getValor().nombre
            << " | FE: " << nodo->getFe() << " }\"];\n";

    if (nodo->getIzq() != nullptr) {
        archivo << "    node" << nodo << " -> node" << nodo->getIzq() << ";\n";
        generarNodosAVL(archivo, nodo->getIzq());
    }

    if (nodo->getDer() != nullptr) {
        archivo << "    node" << nodo << " -> node" << nodo->getDer() << ";\n";
        generarNodosAVL(archivo, nodo->getDer());
    }
}

//////////////////////////////////////////////////////////////
// ARBOL B
//////////////////////////////////////////////////////////////

void ReporteGraficos::generarArbolB(ArbolB& arbol) {

    system("mkdir -p Reportes"); // asegura carpeta

    std::ofstream archivo("Reportes/arbolB.dot");

    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear arbolB.dot\n";
        return;
    }

    archivo << "digraph ArbolB {\n";
    archivo << "    node [shape=record, style=filled, fillcolor=lightyellow];\n";
    archivo << "    label=\"Árbol B (d=2)\";\n";

    generarNodosB(archivo, arbol.getRaiz());

    archivo << "}\n";
    archivo.close();

    std::cout << "[INFO] Generando imagen Arbol B...\n";
    system("dot -Tpng Reportes/arbolB.dot -o Reportes/arbolB.png");
    std::cout << "[INFO] Imagen Reportes/arbolB.png generada con éxito.\n";
}

void ReporteGraficos::generarNodosB(std::ofstream& archivo, NodoArbolB* nodo) {
    if (nodo == nullptr) return;

    // ✅ Nodo tipo tabla
    archivo << "    node" << nodo << " [label=\"";

    for (int i = 0; i < nodo->getNumLlaves(); i++) {
        archivo << "<f" << i << "> " << nodo->getLlave(i).nombre;
        if (i < nodo->getNumLlaves() - 1)
            archivo << " | ";
    }

    archivo << " | <f" << nodo->getNumLlaves() << ">\"];\n";

    // ✅ Conexiones
    if (!nodo->getEsHoja()) {
        for (int i = 0; i <= nodo->getNumLlaves(); i++) {
            if (nodo->getHijo(i) != nullptr) {
                archivo << "    node" << nodo << ":f" << i
                    << " -> node" << nodo->getHijo(i) << ";\n";

                generarNodosB(archivo, nodo->getHijo(i));
            }
        }
    }
}
