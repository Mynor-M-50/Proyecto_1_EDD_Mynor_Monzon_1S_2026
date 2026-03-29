//
// Created by mynorm50 on 7/3/26.
//

#include "ReporteGraficos.h"
#include "../Estructuras/NodoArbolAVL.h"
#include "../Estructuras/NodoArbolB.h"
#include "../Estructuras/NodoArbolBPlus.h"
#include <fstream>
#include <cstdlib>

//////////////////////////////////////////////////////////////
//                         AVL
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
//                          ARBOL B
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

    // nodo tipo tabla
    archivo << "    node" << nodo << " [label=\"";

    for (int i = 0; i < nodo->getNumLlaves(); i++) {
        archivo << "<f" << i << "> " << nodo->getLlave(i).nombre;
        if (i < nodo->getNumLlaves() - 1)
            archivo << " | ";
    }

    archivo << " | <f" << nodo->getNumLlaves() << ">\"];\n";

    // conexiones
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

//////////////////////////////////////////////////////////////
//                      ARBOL B Plus
//////////////////////////////////////////////////////////////

void ReporteGraficos::generarArbolBPlus(ArbolBPlus &arbol) {
    system("mkdir -p Reportes");
    std::ofstream archivo("Reportes/arbolBPlus.dot");

    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear arbolBPlus.dot\n";
        return;
    }

    archivo << "digraph ArbolBPlus {\n";

    archivo << "    node [shape=record, style=filled, fillcolor=lightgreen];\n";
    archivo << "    label=\"Arbol B+ (d=2)\";\n";
    archivo << "    rankdir=TB;\n";

    // dibujar todos los nodos
    generarNodosBPlus(archivo, arbol.getRaiz());

    archivo << "}\n";
    archivo.close();

    std::cout << "[INFO] Generando imagen Arbol BPlus...\n";
    system("dot -Tpng Reportes/arbolBPlus.dot -o Reportes/arbolBPlus.png");
    std::cout << "[INFO] Imagen Reportes/arbolBPlus.png generada con exito.\n";
}

void ReporteGraficos::generarNodosBPlus(std::ofstream &archivo, NodoArbolBPlus *nodo) {
    if (nodo == nullptr) return;

    // Color segun si es hoja o no
    std::string color = nodo->getEsHoja() ? "palegreen" : "lightgreen";

    archivo << "    node" << nodo << " [fillcolor=" << color << ", label=\"";

    for (int i = 0; i < nodo->getNumLlaves(); i++) {
        archivo << "<f" << i << ">" << nodo->getLlave(i).nombre;
        archivo << "|";
    }
    archivo << "<f" << nodo->getNumLlaves() << ">\"];\n";

    // conexiones con hijos
    if (!nodo->getEsHoja()) {
        for (int i = 0; i <= nodo->getNumLlaves(); i++) {
            if (nodo->getHijo(i) != nullptr) {
                archivo << "    node" << nodo << ":f" << i
                        << " -> node" << nodo->getHijo(i) << ";\n";
                generarNodosBPlus(archivo, nodo->getHijo(i));
            }
        }
    }
}

//////////////////////////////////////////////////////////////
//                      Tabla Hash
//////////////////////////////////////////////////////////////

void ReporteGraficos::generarTablaHash(TablaHash& hash) {
    system("mkdir -p Reportes");
    std::ofstream archivo("Reportes/tablaHash.dot");

    archivo << "digraph TablaHash {\n";
    archivo << "    rankdir=LR;\n"; // De izquierda a derecha
    archivo << "    node [shape=record];\n";
    archivo << "    label=\"Tabla Hash (Encadenamiento)\";\n";

    // 1 .dibujar la columna de indices (el arreglo principal)
    archivo << "    indices [label=\"";
    bool primero = true;
    for (int i = 0; i < hash.getTamanio(); i++) {
        if (hash.getCubeta(i)->getHead() != nullptr) {
            if (!primero) archivo << " | ";
            archivo << "<f" << i << "> [" << i << "]";
            primero = false;
        }
    }
    archivo << "\"];\n";

    // 2. dibujar las listas para cada cubeta que no est vacia
    for (int i = 0; i < hash.getTamanio(); i++) {
        NodoListaEnlazada<Producto>* actual = hash.getCubeta(i)->getHead();

        if (actual != nullptr) {
            // Conectar el indice con el primer nodo de la lista
            archivo << "    indices:f" << i << " -> node" << actual << ";\n";

            while (actual != nullptr) {
                // dibujar el nodo del producto
                archivo << "    node" << actual << " [label=\"{ "
                        << actual->getValor().nombre << " | "
                        << actual->getValor().codigoBarras << " }\"];\n";

                // conectar con el siguiente si existe
                if (actual->getSiguiente() != nullptr) {
                    archivo << "    node" << actual << " -> node" << actual->getSiguiente() << ";\n";
                }
                actual = actual->getSiguiente();
            }
        }
    }

    archivo << "}\n";
    archivo.close();

    std::cout << "[INFO] Generando imagen Tabla Hash...\n";
    system("dot -Tpng Reportes/tablaHash.dot -o Reportes/tablaHash.png");
    std::cout << "[INFO] Imagen Reportes/tablaHash.png generada con exito.\n";
}