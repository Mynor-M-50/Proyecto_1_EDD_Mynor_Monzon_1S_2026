//
// Created by mynorm50 on 16/3/26.
//

#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>

struct Producto {
    std::string nombre;
    std::string codigoBarras;
    std::string categoria;
    std::string fechaVencimiento; // Formato YYYY-MM-DD
    std::string marca;
    double precio;
    int stock;

    // constructor vacio
    Producto() : precio(0.0), stock(0) {}

    // Constructor con parámetros
    Producto(std::string n, std::string cb, std::string cat, std::string fv, std::string m, double p, int s)
        : nombre(n), codigoBarras(cb), categoria(cat), fechaVencimiento(fv), marca(m), precio(p), stock(s) {}
};

#endif