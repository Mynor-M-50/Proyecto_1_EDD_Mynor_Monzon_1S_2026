//
// Created by mynorm50 on 16/3/26.
//

#ifndef CATALOGO_H
#define CATALOGO_H

#include "../Modelo/Producto.h"
#include "../Estructuras/ListaEnlazada.h"
#include "../Estructuras/ArbolAVL.h"
#include "../Estructuras/ArbolB.h"
#include "../Estructuras/ArbolBPlus.h"
#include "../Estructuras/TablaHash.h"

class Catalogo {
private:
    ListaEnlazada<Producto> lista;
    ListaEnlazada<Producto> listaOrdenada;
    ArbolAVL avl;
    ArbolB arbolB;
    ArbolBPlus arbolBPlus;
    TablaHash hash;

public:
    Catalogo();

    bool agregarProducto(const Producto& p);

    Producto* buscarPorCodigo(std::string codigo);

    void imprimirResumen();
};

#endif