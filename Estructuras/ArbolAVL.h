//
// Created by mynorm50 on 7/3/26.
//

#ifndef ARBOL_AVL_H
#define ARBOL_AVL_H

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include "NodoArbolAVL.h"
#include "../Modelo/Producto.h"

class ArbolAVL {
private:
    NodoArbolAVL* raiz;

    // utilidad
    std::string toLower(std::string s) const {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    int altura(NodoArbolAVL* nodo) const {
        if (nodo == nullptr) return 0;
        int altIzq = altura(nodo->getIzq());
        int altDer = altura(nodo->getDer());
        return 1 + (altIzq > altDer ? altIzq : altDer);
    }

    void actualizarFE(NodoArbolAVL* nodo) {
        if (nodo == nullptr) return;
        nodo->setFe(altura(nodo->getDer()) - altura(nodo->getIzq()));
    }

    // rotaciones

    // rotacion II:
    NodoArbolAVL* rotacionII(NodoArbolAVL* nodo) {
        NodoArbolAVL* nodo1 = nodo->getIzq();
        nodo->setIzq(nodo1->getDer());
        nodo1->setDer(nodo);
        actualizarFE(nodo);
        actualizarFE(nodo1);
        return nodo1;
    }

    // rotacion DD:
    NodoArbolAVL* rotacionDD(NodoArbolAVL* nodo) {
        NodoArbolAVL* nodo1 = nodo->getDer();
        nodo->setDer(nodo1->getIzq());
        nodo1->setIzq(nodo);
        actualizarFE(nodo);
        actualizarFE(nodo1);
        return nodo1;
    }

    // rotacion ID:
    NodoArbolAVL* rotacionID(NodoArbolAVL* nodo) {
        NodoArbolAVL* nodo1 = nodo->getIzq();
        NodoArbolAVL* nodo2 = nodo1->getDer();

        nodo1->setDer(nodo2->getIzq());
        nodo2->setIzq(nodo1);
        nodo->setIzq(nodo2->getDer());
        nodo2->setDer(nodo);

        actualizarFE(nodo);
        actualizarFE(nodo1);
        actualizarFE(nodo2);
        return nodo2;
    }

    // rotacion DI:
    NodoArbolAVL* rotacionDI(NodoArbolAVL* nodo) {
        NodoArbolAVL* nodo1 = nodo->getDer();
        NodoArbolAVL* nodo2 = nodo1->getIzq();

        nodo1->setIzq(nodo2->getDer());
        nodo2->setDer(nodo1);
        nodo->setDer(nodo2->getIzq());
        nodo2->setIzq(nodo);

        actualizarFE(nodo);
        actualizarFE(nodo1);
        actualizarFE(nodo2);
        return nodo2;
    }

    // balancear
    NodoArbolAVL* balancear(NodoArbolAVL* nodo) {
        actualizarFE(nodo);

        // desbalance a la derecha
        if (nodo->getFe() == 2) {
            if (nodo->getDer()->getFe() >= 0) {
                return rotacionDD(nodo); // DD
            } else {
                return rotacionDI(nodo); // DI
            }
        }

        // desbalance a la izquierda
        if (nodo->getFe() == -2) {
            if (nodo->getIzq()->getFe() <= 0) {
                return rotacionII(nodo); // II
            } else {
                return rotacionID(nodo); // ID
            }
        }

        return nodo; // Ya está balanceado
    }

    // insertar recursivo
    NodoArbolAVL* insertar(NodoArbolAVL* nodo, const Producto& p, bool& insertado) {
        if (nodo == nullptr) {
            insertado = true;
            return new NodoArbolAVL(p);
        }

        std::string nuevoNombre = toLower(p.nombre);
        std::string actualNombre = toLower(nodo->getValor().nombre);

        if (nuevoNombre < actualNombre) {
            nodo->setIzq(insertar(nodo->getIzq(), p, insertado));
        } else if (nuevoNombre > actualNombre) {
            nodo->setDer(insertar(nodo->getDer(), p, insertado));
        } else {
            // Nombre duplicado → no se inserta
            insertado = false;
            return nodo;
        }

        return balancear(nodo);
    }

    // buscar recursivo
    NodoArbolAVL* buscar(NodoArbolAVL* nodo, const std::string& nombre) const {
        if (nodo == nullptr) return nullptr;

        std::string buscarNombre = toLower(nombre);
        std::string actualNombre = toLower(nodo->getValor().nombre);

        if (buscarNombre == actualNombre) return nodo;
        if (buscarNombre < actualNombre) return buscar(nodo->getIzq(), nombre);
        return buscar(nodo->getDer(), nombre);
    }

    // nodo a mas izquierda para eliominar
    NodoArbolAVL* minimoNodo(NodoArbolAVL* nodo) const {
        while (nodo->getIzq() != nullptr) {
            nodo = nodo->getIzq();
        }
        return nodo;
    }

    // eliminar recursivo
    NodoArbolAVL* eliminar(NodoArbolAVL* nodo, const std::string& nombre, bool& eliminado) {
        if (nodo == nullptr) {
            eliminado = false;
            return nullptr;
        }

        std::string buscarNombre = toLower(nombre);
        std::string actualNombre = toLower(nodo->getValor().nombre);

        if (buscarNombre < actualNombre) {
            nodo->setIzq(eliminar(nodo->getIzq(), nombre, eliminado));
        } else if (buscarNombre > actualNombre) {
            nodo->setDer(eliminar(nodo->getDer(), nombre, eliminado));
        } else {
            eliminado = true;

            // Caso 1: hoja
            if (nodo->getIzq() == nullptr && nodo->getDer() == nullptr) {
                delete nodo;
                return nullptr;
            }
            // Caso 2: solo hijo derecho
            if (nodo->getIzq() == nullptr) {
                NodoArbolAVL* temp = nodo->getDer();
                delete nodo;
                return temp;
            }
            // Caso 3: solo hijo izquierdo
            if (nodo->getDer() == nullptr) {
                NodoArbolAVL* temp = nodo->getIzq();
                delete nodo;
                return temp;
            }
            // Caso 4: dos hijos -> reemplazar con mnimo del subarbol derecho
            NodoArbolAVL* sucesor = minimoNodo(nodo->getDer());
            nodo->setValor(sucesor->getValor());
            bool aux = false;
            nodo->setDer(eliminar(nodo->getDer(), sucesor->getValor().nombre, aux));
        }

        return balancear(nodo);
    }

    // destructor recursivo
    void destruir(NodoArbolAVL* nodo) {
        if (nodo == nullptr) return;
        destruir(nodo->getIzq());
        destruir(nodo->getDer());
        delete nodo;
    }

    // imprimir oredenado por inorden
    void inorden(NodoArbolAVL* nodo) const {
        if (nodo == nullptr) return;
        inorden(nodo->getIzq());
        std::cout << nodo->getValor().nombre
            << " (FE: " << nodo->getFe() << ")\n";
        inorden(nodo->getDer());
    }

public:
    ArbolAVL() : raiz(nullptr) {}

    ~ArbolAVL() {
        destruir(raiz);
    }

    bool insertar(const Producto& p) {
        bool insertado = false;
        raiz = insertar(raiz, p, insertado);
        return insertado;
    }

    bool eliminar(const std::string& nombre) {
        bool eliminado = false;
        raiz = eliminar(raiz, nombre, eliminado);
        return eliminado;
    }

    Producto* buscar(const std::string& nombre) {
        NodoArbolAVL* resultado = buscar(raiz, nombre);
        if (resultado == nullptr) return nullptr;
        return &(resultado->getValor());
    }

    bool isEmpty() const { return raiz == nullptr; }

    NodoArbolAVL* getRaiz() const { return raiz; }

    void imprimirInorden() const {
        inorden(raiz);
    }
};

#endif