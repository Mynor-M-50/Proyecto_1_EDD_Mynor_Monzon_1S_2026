//
// Created by mynorm50 on 29/3/26.
//

#ifndef PILA_H
#define PILA_H

#include "NodoListaEnlazada.h"
#include <iostream>

template <typename T>
class Pila {
private:
    NodoListaEnlazada<T>* tope;
    int tamanio;

public:
    Pila() : tope(nullptr), tamanio(0) {}

    ~Pila() {
        while (!estaVacia()) {
            pop();
        }
    }

    // Push: Insertar al principio (O(1))
    void push(T valor) {
        NodoListaEnlazada<T>* nuevo = new NodoListaEnlazada<T>(valor);
        nuevo->setSiguiente(tope);
        tope = nuevo;
        tamanio++;
    }

    // Pop: Eliminar del principio (O(1))
    T pop() {
        if (estaVacia()) {
            throw std::runtime_error("Pila vacia");
        }
        NodoListaEnlazada<T>* temp = tope;
        T valor = temp->getValor();
        tope = tope->getSiguiente();
        delete temp;
        tamanio--;
        return valor;
    }

    T peek() {
        if (estaVacia()) throw std::runtime_error("Pila vacia");
        return tope->getValor();
    }

    bool estaVacia() {
        return tope == nullptr;
    }

    int getTamanio() {
        return tamanio;
    }
};

#endif