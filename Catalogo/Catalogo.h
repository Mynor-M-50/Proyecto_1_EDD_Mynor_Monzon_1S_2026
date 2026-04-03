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
#include "../Estructuras/Pila.h"

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