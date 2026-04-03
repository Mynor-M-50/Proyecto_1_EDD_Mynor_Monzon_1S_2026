//
// Created by mynorm50 on 7/3/26.
//

#ifndef LISTA_ENLAZADA_H
#define LISTA_ENLAZADA_H

#include <iostream>
#include <string>
#include "NodoListaEnlazada.h"
#include <algorithm>
#include <cctype>

template <typename T>
class ListaEnlazada {
private:
    NodoListaEnlazada<T>* head;
    int size;

    // Convierte un string a minusculas para comparaciones case-insensitive
    std::string toLower(std::string s) const {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

public:
    ListaEnlazada() : head(nullptr), size(0) {}

    ~ListaEnlazada() {
        while (!isEmpty()) {
            deleteFirst();
        }
    }

    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }
    NodoListaEnlazada<T>* getHead() const { return head; }

    // -Para lista no ordenada-
    void insertarAlFinal(const T& valor) {
        if (head == nullptr) {
            head = new NodoListaEnlazada<T>(valor);
        } else {
            NodoListaEnlazada<T>* actual = head;
            while (actual->getSiguiente() != nullptr) {
                actual = actual->getSiguiente();
            }
            actual->setSiguiente(new NodoListaEnlazada<T>(valor));
        }
        size++;
    }

    // Para lista ordenada por nombre
    // Solo funcionará si T es Producto o tiene el atributo .nombre
    void insertarOrdenadoPorNombre(const T& valor) {
        NodoListaEnlazada<T>* nuevo = new NodoListaEnlazada<T>(valor);

        // Caso 1: Lista vacia o el nuevo va antes que el head
        if (head == nullptr || toLower(valor.nombre) < toLower(head->getValor().nombre)) {
            nuevo->setSiguiente(head);
            head = nuevo;
        } else {
            // Caso 2: Buscar la posicion correcta
            NodoListaEnlazada<T>* actual = head;
            while (actual->getSiguiente() != nullptr &&
                toLower(actual->getSiguiente()->getValor().nombre) < toLower(valor.nombre)) {
                actual = actual->getSiguiente();
            }
            nuevo->setSiguiente(actual->getSiguiente());
            actual->setSiguiente(nuevo);
        }
        size++;
    }

    void deleteFirst() {
        if (isEmpty()) return;
        NodoListaEnlazada<T>* temp = head;
        head = head->getSiguiente();
        delete temp;
        size--;
    }

    // Buscar por codigo de barras
    T* buscarPorCodigo(std::string codigo) {
        NodoListaEnlazada<T>* actual = head;
        while (actual != nullptr) {
            if (actual->getValor().codigoBarras == codigo) {
                return &(actual->getValor());
            }
            actual = actual->getSiguiente();
        }
        return nullptr;
    }

    // Eliminar por codigo de barras (para el rollback)
    bool eliminarPorCodigo(std::string codigo) {
        if (isEmpty()) return false;

        if (head->getValor().codigoBarras == codigo) {
            deleteFirst();
            return true;
        }

        NodoListaEnlazada<T>* actual = head;
        while (actual->getSiguiente() != nullptr) {
            if (actual->getSiguiente()->getValor().codigoBarras == codigo) {
                NodoListaEnlazada<T>* aEliminar = actual->getSiguiente();
                actual->setSiguiente(aEliminar->getSiguiente());
                delete aEliminar;
                size--;
                return true;
            }
            actual = actual->getSiguiente();
        }
        return false;
    }

    // Para prueba rapida
    void imprimirNombres() const {
        NodoListaEnlazada<T>* actual = head;
        while (actual != nullptr) {
            std::cout << "- " << actual->getValor().nombre
                << " [" << actual->getValor().codigoBarras << "]\n";
            actual = actual->getSiguiente();
        }
    }

};

#endif