//
// Created by mynorm50 on 7/3/26.
//

#ifndef NODO_ARBOL_B_PLUS_H
#define NODO_ARBOL_B_PLUS_H

#include "Producto.h"

const int ORDEN_BP = 2;
const int MAX_LLAVES_BP = 2 * ORDEN_BP;
const int MAX_HIJOS_BP = MAX_LLAVES_BP + 1;

class NodoArbolBPlus {
private:
    Producto llaves[MAX_LLAVES_BP];
    NodoArbolBPlus* hijos[MAX_HIJOS_BP];
    int numLlaves;
    bool esHoja;

    // B+: Puntero a la siguiente hoja
    NodoArbolBPlus* siguiente;

public:
    NodoArbolBPlus(bool hoja = true) : numLlaves(0), esHoja(hoja), siguiente(nullptr) {
        for (int i = 0; i < MAX_HIJOS_BP; i++) {
            hijos[i] = nullptr;
        }
    }

    // getters y setters
    Producto& getLlave(int i) { return llaves[i]; }
    NodoArbolBPlus* getHijo(int i) const { return hijos[i]; }
    int getNumLlaves() const { return numLlaves; }
    bool getEsHoja() const { return esHoja; }
    NodoArbolBPlus* getSiguiente() const { return siguiente; }

    void setLlave(int i, const Producto& p) { llaves[i] = p; }
    void setHijo(int i, NodoArbolBPlus* n) { hijos[i] = n; }
    void setNumLlaves(int n) { numLlaves = n; }
    void setEsHoja(bool h) { esHoja = h; }
    void setSiguiente(NodoArbolBPlus* s) { siguiente = s; }

    bool estaLleno() const { return numLlaves == MAX_LLAVES_BP; }
};

#endif