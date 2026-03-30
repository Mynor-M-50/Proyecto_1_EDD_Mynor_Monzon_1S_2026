//
// Created by mynorm50 on 7/3/26.
//

#ifndef REPORTE_GRAFICOS_H
#define REPORTE_GRAFICOS_H

#include "../Estructuras/ArbolAVL.h"
#include "../Estructuras/ArbolB.h"
#include "../Estructuras/ArbolBPlus.h"
#include "../Estructuras/TablaHash.h"
#include "../Estructuras/NodoListaEnlazada.h"
#include <fstream>
#include <string>

class ReporteGraficos {
public:
    void generarAVL(ArbolAVL& arbol,          const std::string& nombre = "avl");
    void generarArbolB(ArbolB& arbol,          const std::string& nombre = "arbolB");
    void generarArbolBPlus(ArbolBPlus& arbol,  const std::string& nombre = "arbolBPlus");
    void generarTablaHash(TablaHash& hash,     const std::string& nombre = "tablaHash");

private:
    void generarNodosAVL(std::ofstream& archivo, NodoArbolAVL* nodo);
    void generarNodosB(std::ofstream& archivo, NodoArbolB* nodo);
    void generarNodosBPlus(std::ofstream& archivo, NodoArbolBPlus* nodo);
};

#endif