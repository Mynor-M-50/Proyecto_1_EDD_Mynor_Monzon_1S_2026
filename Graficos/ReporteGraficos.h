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

class ReporteGraficos {
public:
    void generarAVL(ArbolAVL& arbol);
    void generarArbolB(ArbolB& arbol);
    void generarArbolBPlus(ArbolBPlus& arbol);
    void generarTablaHash(TablaHash& hash);

private:
    void generarNodosAVL(std::ofstream& archivo, NodoArbolAVL* nodo);
    void generarNodosB(std::ofstream& archivo, NodoArbolB* nodo);
    void generarNodosBPlus(std::ofstream& archivo, NodoArbolBPlus* nodo);
};

#endif