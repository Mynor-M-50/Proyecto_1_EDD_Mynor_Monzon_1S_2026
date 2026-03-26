//
// Created by mynorm50 on 7/3/26.
//

#ifndef ARBOL_B_PLUS_H
#define ARBOL_B_PLUS_H

#include <iostream>
#include <algorithm>
#include <string>
#include "NodoArbolBPlus.h"
#include "../Modelo/Producto.h"

class ArbolBPlus {
private:
    NodoArbolBPlus* raiz;

    // Utilidad para minúsculas
    std::string toLower(std::string s) const {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    int comparar(const std::string& a, const std::string& b) const {
        std::string la = toLower(a);
        std::string lb = toLower(b);
        if (la < lb) return -1;
        if (la > lb) return  1;
        return 0;
    }

    // split especial de B+
    void split(NodoArbolBPlus* padre, int i, NodoArbolBPlus* hijoLleno) {
        NodoArbolBPlus* nuevoNodo = new NodoArbolBPlus(hijoLleno->getEsHoja());
        int medio = ORDEN_BP; // d = 2

        if (hijoLleno->getEsHoja()) {
            // split de hoja
            // enn B+, la hoja derecha se queda con d+1 llaves (incluyendo la que sube
            nuevoNodo->setNumLlaves(hijoLleno->getNumLlaves() - medio);
            for (int j = 0; j < nuevoNodo->getNumLlaves(); j++) {
                nuevoNodo->setLlave(j, hijoLleno->getLlave(j + medio));
            }
            hijoLleno->setNumLlaves(medio);

            // enlazar hojas (Lista Enlazada)
            nuevoNodo->setSiguiente(hijoLleno->getSiguiente());
            hijoLleno->setSiguiente(nuevoNodo);

            // subir copia de la primera llave del nuevo nodo al padre
            subirLlave(padre, i, nuevoNodo->getLlave(0), nuevoNodo);
        } else {
            // split de nodo interno
            // Igual que el arbol B normal
            nuevoNodo->setNumLlaves(hijoLleno->getNumLlaves() - medio - 1);
            for (int j = 0; j < nuevoNodo->getNumLlaves(); j++) {
                nuevoNodo->setLlave(j, hijoLleno->getLlave(j + medio + 1));
            }
            for (int j = 0; j <= nuevoNodo->getNumLlaves(); j++) {
                nuevoNodo->setHijo(j, hijoLleno->getHijo(j + medio + 1));
            }

            Producto llaveSubir = hijoLleno->getLlave(medio);
            hijoLleno->setNumLlaves(medio);
            subirLlave(padre, i, llaveSubir, nuevoNodo);
        }
    }

    void subirLlave(NodoArbolBPlus* padre, int i, const Producto& p, NodoArbolBPlus* nuevoHijo) {
        // Mover hijos a la derecha
        for (int j = padre->getNumLlaves(); j >= i + 1; j--) {
            padre->setHijo(j + 1, padre->getHijo(j));
        }
        padre->setHijo(i + 1, nuevoHijo);

        // Mover llaves a la derecha
        for (int j = padre->getNumLlaves() - 1; j >= i; j--) {
            padre->setLlave(j + 1, padre->getLlave(j));
        }
        padre->setLlave(i, p);
        padre->setNumLlaves(padre->getNumLlaves() + 1);
    }

    void insertarNoLleno(NodoArbolBPlus* nodo, const Producto& p) {
        int i = nodo->getNumLlaves() - 1;

        if (nodo->getEsHoja()) {
            while (i >= 0 && comparar(p.nombre, nodo->getLlave(i).nombre) < 0) {
                nodo->setLlave(i + 1, nodo->getLlave(i));
                i--;
            }
            nodo->setLlave(i + 1, p);
            nodo->setNumLlaves(nodo->getNumLlaves() + 1);
        } else {
            while (i >= 0 && comparar(p.nombre, nodo->getLlave(i).nombre) < 0) {
                i--;
            }
            i++;
            if (nodo->getHijo(i)->estaLleno()) {
                split(nodo, i, nodo->getHijo(i));
                if (comparar(p.nombre, nodo->getLlave(i).nombre) > 0) {
                    i++;
                }
            }
            insertarNoLleno(nodo->getHijo(i), p);
        }
    }

public:
    ArbolBPlus() : raiz(nullptr) {}

    void insertar(const Producto& p) {
        if (raiz == nullptr) {
            raiz = new NodoArbolBPlus(true);
            raiz->setLlave(0, p);
            raiz->setNumLlaves(1);
            return;
        }

        if (raiz->estaLleno()) {
            NodoArbolBPlus* nuevaRaiz = new NodoArbolBPlus(false);
            nuevaRaiz->setHijo(0, raiz);
            split(nuevaRaiz, 0, raiz);
            raiz = nuevaRaiz;
        }
        insertarNoLleno(raiz, p);
    }

    // recorrido por hojas (Lineal)
    void imprimirLineal() const {
        if (raiz == nullptr) return;

        // 1. Bajar hasta la primera hoja (la más a la izquierda)
        NodoArbolBPlus* actual = raiz;
        while (!actual->getEsHoja()) {
            actual = actual->getHijo(0);
        }

        // 2. Recorrer usando el puntero 'siguiente'
        while (actual != nullptr) {
            for (int i = 0; i < actual->getNumLlaves(); i++) {
                std::cout << actual->getLlave(i).nombre << " [B+ Leaf]\n";
            }
            actual = actual->getSiguiente();
        }
    }

    Producto* buscar(const std::string& nombre) {
        if (raiz == nullptr) return nullptr;
        NodoArbolBPlus* actual = raiz;

        while (!actual->getEsHoja()) {
            int i = 0;
            while (i < actual->getNumLlaves() && comparar(nombre, actual->getLlave(i).nombre) >= 0) {
                i++;
            }
            actual = actual->getHijo(i);
        }

        for (int i = 0; i < actual->getNumLlaves(); i++) {
            if (comparar(nombre, actual->getLlave(i).nombre) == 0) {
                return &(actual->getLlave(i));
            }
        }
        return nullptr;
    }
};

#endif