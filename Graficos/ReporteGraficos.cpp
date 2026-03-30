//
// Created by mynorm50 on 7/3/26.
//

#include "ReporteGraficos.h"
#include "../Estructuras/NodoArbolAVL.h"
#include "../Estructuras/NodoArbolB.h"
#include "../Estructuras/NodoArbolBPlus.h"
#include <fstream>
#include <cstdlib>
#include <string>

//////////////////////////////////////////////////////////////
//                         AVL
//////////////////////////////////////////////////////////////

void ReporteGraficos::generarAVL(ArbolAVL& arbol, const std::string& nombre) {
    system("mkdir -p Reportes");

    std::string dotFile = "Reportes/" + nombre + ".dot";
    std::string pngFile = "Reportes/" + nombre + ".png";

    std::ofstream archivo(dotFile);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear " << dotFile << "\n";
        return;
    }

    archivo << "digraph AVL {\n";
    archivo << "    node [shape=record, style=filled, fillcolor=lightblue];\n";
    archivo << "    label=\"Arbol AVL de Productos\";\n";

    generarNodosAVL(archivo, arbol.getRaiz());

    archivo << "}\n";
    archivo.close();

    std::string cmd = "dot -Tpng " + dotFile + " -o " + pngFile;
    system(cmd.c_str());
    std::cout << "[INFO] Imagen " << pngFile << " generada con exito.\n";
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

void ReporteGraficos::generarArbolB(ArbolB& arbol, const std::string& nombre) {
    system("mkdir -p Reportes");

    std::string dotFile = "Reportes/" + nombre + ".dot";
    std::string pngFile = "Reportes/" + nombre + ".png";

    std::ofstream archivo(dotFile);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear " << dotFile << "\n";
        return;
    }

    archivo << "digraph ArbolB {\n";
    archivo << "    node [shape=record, style=filled, fillcolor=lightyellow];\n";
    archivo << "    label=\"Arbol B (d=2)\";\n";

    generarNodosB(archivo, arbol.getRaiz());

    archivo << "}\n";
    archivo.close();

    std::string cmd = "dot -Tpng " + dotFile + " -o " + pngFile;
    system(cmd.c_str());
    std::cout << "[INFO] Imagen " << pngFile << " generada con exito.\n";
}

void ReporteGraficos::generarNodosB(std::ofstream& archivo, NodoArbolB* nodo) {
    if (nodo == nullptr) return;

    archivo << "    node" << nodo << " [label=\"";

    for (int i = 0; i < nodo->getNumLlaves(); i++) {
        archivo << "<f" << i << "> " << nodo->getLlave(i).nombre;
        if (i < nodo->getNumLlaves() - 1)
            archivo << " | ";
    }

    archivo << " | <f" << nodo->getNumLlaves() << ">\"];\n";

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

void ReporteGraficos::generarArbolBPlus(ArbolBPlus& arbol, const std::string& nombre) {
    system("mkdir -p Reportes");

    std::string dotFile = "Reportes/" + nombre + ".dot";
    std::string pngFile = "Reportes/" + nombre + ".png";

    std::ofstream archivo(dotFile);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear " << dotFile << "\n";
        return;
    }

    archivo << "digraph ArbolBPlus {\n";
    archivo << "    label=\"Arbol B+ (d=2)\";\n";
    archivo << "    rankdir=TB;\n";

    generarNodosBPlus(archivo, arbol.getRaiz());

    NodoArbolBPlus* hoja = arbol.getRaiz();
    if (hoja != nullptr) {
        while (!hoja->getEsHoja()) {
            hoja = hoja->getHijo(0);
        }

        archivo << "\n    // Enlace entre hojas\n";
        archivo << "    edge [color=red, style=dashed, constraint=false, weight=0];\n";

        NodoArbolBPlus* temp = hoja;
        archivo << "    { rank=same; ";
        while (temp != nullptr) {
            archivo << "node" << temp << "; ";
            temp = temp->getSiguiente();
        }
        archivo << "}\n";

        while (hoja != nullptr && hoja->getSiguiente() != nullptr) {
            archivo << "    node" << hoja
                    << " -> node" << hoja->getSiguiente()
                    << " [dir=forward];\n";
            hoja = hoja->getSiguiente();
        }

        archivo << "    edge [color=black, style=solid, constraint=true];\n";
    }

    archivo << "}\n";
    archivo.close();

    std::string cmd = "dot -Tpng " + dotFile + " -o " + pngFile;
    system(cmd.c_str());
    std::cout << "[INFO] Imagen " << pngFile << " generada con exito.\n";
}

void ReporteGraficos::generarNodosBPlus(std::ofstream& archivo, NodoArbolBPlus* nodo) {
    if (nodo == nullptr) return;

    std::string color = nodo->getEsHoja() ? "palegreen" : "lightgreen";

    archivo << "    node" << nodo
            << " [style=filled, fillcolor=" << color
            << ", shape=plaintext, label=<\n"
            << "        <TABLE BORDER=\"1\" CELLBORDER=\"1\" CELLSPACING=\"0\">\n"
            << "        <TR>\n";

    for (int i = 0; i < nodo->getNumLlaves(); i++) {
        archivo << "            <TD PORT=\"f" << i << "\">"
                << nodo->getLlave(i).nombre << "</TD>\n";
    }
    archivo << "            <TD PORT=\"f" << nodo->getNumLlaves() << "\"> </TD>\n";
    archivo << "        </TR>\n"
            << "        </TABLE>>];\n";

    if (!nodo->getEsHoja()) {
        for (int i = 0; i <= nodo->getNumLlaves(); i++) {
            if (nodo->getHijo(i) != nullptr) {
                archivo << "    node" << nodo << ":f" << i
                        << " -> node" << nodo->getHijo(i) << ":f0;\n";
                generarNodosBPlus(archivo, nodo->getHijo(i));
            }
        }
    }
}

//////////////////////////////////////////////////////////////
//                      Tabla Hash
//////////////////////////////////////////////////////////////

void ReporteGraficos::generarTablaHash(TablaHash& hash, const std::string& nombre) {
    system("mkdir -p Reportes");

    std::string dotFile = "Reportes/" + nombre + ".dot";
    std::string pngFile = "Reportes/" + nombre + ".png";

    std::ofstream archivo(dotFile);
    if (!archivo.is_open()) {
        std::cerr << "[ERROR] No se pudo crear " << dotFile << "\n";
        return;
    }

    archivo << "digraph TablaHash {\n";
    archivo << "    rankdir=LR;\n";
    archivo << "    node [shape=record];\n";
    archivo << "    label=\"Tabla Hash (Encadenamiento)\";\n";

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

    for (int i = 0; i < hash.getTamanio(); i++) {
        NodoListaEnlazada<Producto>* actual = hash.getCubeta(i)->getHead();

        if (actual != nullptr) {
            archivo << "    indices:f" << i << " -> node" << actual << ";\n";

            while (actual != nullptr) {
                archivo << "    node" << actual << " [label=\"{ "
                        << actual->getValor().nombre << " | "
                        << actual->getValor().codigoBarras << " }\"];\n";

                if (actual->getSiguiente() != nullptr) {
                    archivo << "    node" << actual << " -> node" << actual->getSiguiente() << ";\n";
                }
                actual = actual->getSiguiente();
            }
        }
    }

    archivo << "}\n";
    archivo.close();

    std::string cmd = "dot -Tpng " + dotFile + " -o " + pngFile;
    system(cmd.c_str());
    std::cout << "[INFO] Imagen " << pngFile << " generada con exito.\n";
}