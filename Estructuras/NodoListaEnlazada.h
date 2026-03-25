//
// Created by mynorm50 on 7/3/26.
//

#ifndef NODO_LISTA_ENLAZADA_H
#define NODO_LISTA_ENLAZADA_H

template <typename T>
class NodoListaEnlazada {
private:
    T valor;
    NodoListaEnlazada<T>* siguiente;

public:
    NodoListaEnlazada(const T& valor) : valor(valor), siguiente(nullptr) {}

    T& getValor() { return valor; }
    const T& getValor() const { return valor; }

    void setValor(const T& nuevoValor) { valor = nuevoValor; }

    NodoListaEnlazada<T>* getSiguiente() const { return siguiente; }
    void setSiguiente(NodoListaEnlazada<T>* sig) { siguiente = sig; }
};

#endif