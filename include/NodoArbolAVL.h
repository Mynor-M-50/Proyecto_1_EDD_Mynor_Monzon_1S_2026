//
// Created by mynorm50 on 7/3/26.
//

#ifndef NODO_ARBOL_AVL_H
#define NODO_ARBOL_AVL_H

#include "Producto.h"

class NodoArbolAVL {
private:
    Producto valor;
    NodoArbolAVL* izq;
    NodoArbolAVL* der;
    int fe; // factor de equilibrio

public:
    NodoArbolAVL(const Producto& p)
        : valor(p), izq(nullptr), der(nullptr), fe(0) {}

    // getters
    Producto& getValor() { return valor; }
    const Producto& getValor() const { return valor; }
    NodoArbolAVL* getIzq() const { return izq; }
    NodoArbolAVL* getDer() const { return der; }
    int getFe() const { return fe; }

    // setters
    void setValor(const Producto& p) { valor = p; }
    void setIzq(NodoArbolAVL* nodo) { izq = nodo; }
    void setDer(NodoArbolAVL* nodo) { der = nodo; }
    void setFe(int f) { fe = f; }
};

#endif