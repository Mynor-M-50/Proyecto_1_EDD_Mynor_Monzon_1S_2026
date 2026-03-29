//
// Created by mynorm50 on 7/3/26.
//

#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <iostream>
#include <string>
#include "ListaEnlazada.h"
#include "../Modelo/Producto.h"

class TablaHash {
private:
    static const int TAMANIO_INICIAL = 101; // Un numero primo ayuda a distribuir mejor
    ListaEnlazada<Producto>* tabla[TAMANIO_INICIAL];

    // Hash simple (Suma de caracteres ASCII % tamano)
    int funcionHash(std::string llave) const {
        unsigned long hash = 5381;
        for (char c : llave) {
            // djb2 algoritmo - muy eficiente
            hash = ((hash << 5) + hash) + c;
        }
        return hash % TAMANIO_INICIAL;
    }

public:
    TablaHash() {
        for (int i = 0; i < TAMANIO_INICIAL; i++) {
            tabla[i] = new ListaEnlazada<Producto>();
        }
    }

    ~TablaHash() {
        for (int i = 0; i < TAMANIO_INICIAL; i++) {
            delete tabla[i];
        }
    }

    void insertar(const Producto& p) {
        int indice = funcionHash(p.codigoBarras);
        // insertamos al final de la lista en esa posicin (para manejo de colisiones)
        tabla[indice]->insertarAlFinal(p);
    }

    Producto* buscar(std::string codigo) {
        int indice = funcionHash(codigo);
        // Buscamos en la lista de esa cubeta
        return tabla[indice]->buscarPorCodigo(codigo);
    }

    void eliminar(std::string codigo) {
        int indice = funcionHash(codigo);
        tabla[indice]->eliminarPorCodigo(codigo);
    }

    void imprimirEstado() {
        std::cout << "--- Estado de la Tabla Hash ---\n";
        for (int i = 0; i < TAMANIO_INICIAL; i++) {
            if (tabla[i]->getHead() != nullptr) {
                std::cout << "Indice [" << i << "]: ";
                tabla[i]->imprimirNombres();
            }
        }
    }

    // Getter para saber cuntas cubetas ocupadas hay
    int getCubitasOcupadas() const {
        int count = 0;
        for (int i = 0; i < TAMANIO_INICIAL; i++) {
            if (tabla[i]->getHead() != nullptr) count++;
        }
        return count;
    }

    // Para que ReporteGraficos pueda acceder
    ListaEnlazada<Producto>* getCubeta(int i) const {
        return tabla[i];
    }

    int getTamanio() const {
        return TAMANIO_INICIAL;
    }
};

#endif