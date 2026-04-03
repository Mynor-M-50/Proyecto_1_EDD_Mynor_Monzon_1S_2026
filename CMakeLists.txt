cmake_minimum_required(VERSION 3.28)
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
        Graficos/ReporteGraficos.h
        Graficos/ReporteGraficos.cpp
        Estructuras/NodoArbolB.h
        Estructuras/NodoArbolAVL.h
        Estructuras/NodoArbolBPlus.h
        Carga/CSVLoader.h
        Carga/CSVLoader.cpp
        Catalogo/Catalogo.h
        Catalogo/Catalogo.cpp
        Modelo/Producto.h
        Utils/Logger.h
        Utils/Logger.cpp
        Utils/Benchmark.h
        Utils/Benchmark.cpp
        main.cpp
        Estructuras/Pila.h
)
