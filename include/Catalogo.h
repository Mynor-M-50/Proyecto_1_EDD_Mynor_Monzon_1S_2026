//
// Created by mynorm50 on 16/3/26.
//

#ifndef CATALOGO_H
#define CATALOGO_H

#include "Producto.h"
#include "ListaEnlazada.h"
#include "ArbolAVL.h"
#include "ArbolB.h"
#include "ArbolBPlus.h"
#include "TablaHash.h"
#include "Pila.h"

class Catalogo {
private:
    ListaEnlazada<Producto> lista;
    ListaEnlazada<Producto> listaOrdenada;
    ArbolAVL avl;
    ArbolB arbolB;
    ArbolBPlus arbolBPlus;
    TablaHash hash;
    Pila<Producto> historialEliminados;

public:
    Catalogo();

    bool agregarProducto(const Producto& p);

    void eliminarProducto(const std::string& codigo);

    void rollback();

    Producto* buscarPorCodigo(std::string codigo);

    Producto* buscarPorNombre(const std::string& nombre);

    void buscarPorCategoria(const std::string& categoria);

    void buscarPorRango(const std::string& desde, const std::string& hasta);

    void imprimirResumen();

    ArbolAVL& getAVL()           { return avl; }
    ArbolB& getArbolB()          { return arbolB; }
    ArbolBPlus& getArbolBPlus()  { return arbolBPlus; }
    TablaHash& getTablaHash()    { return hash; }
    ListaEnlazada<Producto>& getListaOrdenada() { return listaOrdenada; }
};

#endif