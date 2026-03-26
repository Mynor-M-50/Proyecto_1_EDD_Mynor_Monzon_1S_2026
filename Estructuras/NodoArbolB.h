//
// Created by mynorm50 on 7/3/26.
//

#ifndef NODO_ARBOL_B_H
#define NODO_ARBOL_B_H

#include "../Modelo/Producto.h"

const int ORDEN_B = 2;
const int MAX_LLAVES = 2 * ORDEN_B;      // 4 llaves máximo
const int MAX_HIJOS  = MAX_LLAVES + 1;   // 5 hijos máximo

class NodoArbolB {
private:
    Producto llaves[MAX_LLAVES];
    NodoArbolB* hijos[MAX_HIJOS];
    int numLlaves;
    bool esHoja;

public:
    NodoArbolB(bool hoja = true) : numLlaves(0), esHoja(hoja) {
        for (int i = 0; i < MAX_HIJOS; i++) {
            hijos[i] = nullptr;
        }
    }

    // getters
    Producto& getLlave(int i) { return llaves[i]; }
    const Producto& getLlave(int i) const { return llaves[i]; }
    NodoArbolB* getHijo(int i) const { return hijos[i]; }
    int getNumLlaves() const { return numLlaves; }
    bool getEsHoja() const { return esHoja; }

    // setters
    void setLlave(int i, const Producto& p) { llaves[i] = p; }
    void setHijo(int i, NodoArbolB* n) { hijos[i] = n; }
    void setNumLlaves(int n) { numLlaves = n; }
    void setEsHoja(bool h) { esHoja = h; }

    // utilidad
    bool estaLleno() const { return numLlaves == MAX_LLAVES; }
};

#endif