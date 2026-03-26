//
// Created by mynorm50 on 7/3/26.
//

#ifndef ARBOL_B_H
#define ARBOL_B_H

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include "NodoArbolB.h"
#include "../Modelo/Producto.h"

class ArbolB {
private:
    NodoArbolB* raiz;

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

    // split
    void split(NodoArbolB* padre, int i, NodoArbolB* hijoLleno) {
        NodoArbolB* nuevaPagina = new NodoArbolB(hijoLleno->getEsHoja());

        int medio = ORDEN_B; // = 2

        // paggina que obtiene las llaves de la derecha
        int j;
        for (j = 0; j < hijoLleno->getNumLlaves() - medio - 1; j++) {
            nuevaPagina->setLlave(j, hijoLleno->getLlave(j + medio + 1));
        }

        nuevaPagina->setNumLlaves(hijoLleno->getNumLlaves() - medio - 1);

        // si no es hoja copiar hijops
        if (!hijoLleno->getEsHoja()) {
            for (j = 0; j < nuevaPagina->getNumLlaves() + 1; j++) {
                nuevaPagina->setHijo(j, hijoLleno->getHijo(j + medio + 1));
            }
        }

        // reducir hijo izq
        hijoLleno->setNumLlaves(medio);

        // mover hijos a padre
        for (j = padre->getNumLlaves(); j >= i + 1; j--) {
            padre->setHijo(j + 1, padre->getHijo(j));
        }

        padre->setHijo(i + 1, nuevaPagina);

        // mover llaves de padre
        for (j = padre->getNumLlaves() - 1; j >= i; j--) {
            padre->setLlave(j + 1, padre->getLlave(j));
        }

        // subir lllave central
        padre->setLlave(i, hijoLleno->getLlave(medio));
        padre->setNumLlaves(padre->getNumLlaves() + 1);
    }

    // insertar en nodo que no esta lleno
    void insertarNoLleno(NodoArbolB* nodo, const Producto& p) {
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

    // Buscar
    Producto* buscar(NodoArbolB* nodo, const std::string& nombre) {
        if (nodo == nullptr) return nullptr;

        int i = 0;
        while (i < nodo->getNumLlaves() &&
            comparar(nombre, nodo->getLlave(i).nombre) > 0) {
            i++;
        }

        if (i < nodo->getNumLlaves() &&
            comparar(nombre, nodo->getLlave(i).nombre) == 0) {
            return &(nodo->getLlave(i));
        }

        if (nodo->getEsHoja()) return nullptr;

        return buscar(nodo->getHijo(i), nombre);
    }

    // Destructor
    void destruir(NodoArbolB* nodo) {
        if (nodo == nullptr) return;
        if (!nodo->getEsHoja()) {
            for (int i = 0; i <= nodo->getNumLlaves(); i++) {
                destruir(nodo->getHijo(i));
            }
        }
        delete nodo;
    }

    // Inorden
    void inorden(NodoArbolB* nodo) const {
        if (nodo == nullptr) return;

        int i;
        for (i = 0; i < nodo->getNumLlaves(); i++) {
            // Si no es hoja, primero visita el hijo izquierdo de la llave i
            if (!nodo->getEsHoja()) {
                inorden(nodo->getHijo(i));
            }
            // Imprime la llave i
            std::cout << nodo->getLlave(i).nombre << " [FE: B-Tree]" << "\n";
        }

        // Al terminar el ciclo, falta visitar el último hijo (el de la derecha de la última llave)
        if (!nodo->getEsHoja()) {
            inorden(nodo->getHijo(i));
        }
    }
public:
    ArbolB() : raiz(nullptr) {}

    ~ArbolB() { destruir(raiz); }

    bool insertar(const Producto& p) {
        if (raiz == nullptr) {
            raiz = new NodoArbolB(true);
            raiz->setLlave(0, p);
            raiz->setNumLlaves(1);
            return true;
        }

        if (raiz->estaLleno()) {
            NodoArbolB* nuevaRaiz = new NodoArbolB(false);
            nuevaRaiz->setHijo(0, raiz);
            split(nuevaRaiz, 0, raiz);
            raiz = nuevaRaiz;
        }

        insertarNoLleno(raiz, p);
        return true;
    }

    Producto* buscar(const std::string& nombre) {
        return buscar(raiz, nombre);
    }

    bool isEmpty() const { return raiz == nullptr; }
    NodoArbolB* getRaiz() const { return raiz; }

    void imprimirInorden() const { inorden(raiz); }
};

#endif