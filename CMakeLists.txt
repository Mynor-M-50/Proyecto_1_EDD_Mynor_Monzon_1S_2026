cmake_minimum_required(VERSION 4.1)
project(Proyecto_1)

set(CMAKE_CXX_STANDARD 14)

include_directories(Estructuras)

add_executable(Proyecto_1
        Estructuras/ArbolBPlus.h
        Estructuras/ArbolB.h
        Estructuras/ArbolAVL.h
        Estructuras/ListaEnlazada.h
        Estructuras/NodoListaEnlazada.h
        Estructuras/TablaHash.h
        main.cpp
        Graficos/ReporteGraficos.h
        Graficos/ReporteGraficos.cpp
        Estructuras/NodoArbolB.h
        Estructuras/NodoArbolAVL.h
        Estructuras/NodoArbolBPlus.h)
