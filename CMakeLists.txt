cmake_minimum_required(VERSION 3.28)
project(Proyecto_1)

set(CMAKE_CXX_STANDARD 14)

# Esto le dice a C++ que busque los archivos .h dentro de la carpeta 'include'
include_directories(include)

# Aquí listamos dónde están ahora tus archivos reales (.cpp y .h)
add_executable(Proyecto_1
        main.cpp
        src/ReporteGraficos.cpp
        src/CSVLoader.cpp
        src/Catalogo.cpp
        src/Logger.cpp
        src/Benchmark.cpp
        include/ArbolBPlus.h
        include/ArbolB.h
        include/ArbolAVL.h
        include/ListaEnlazada.h
        include/NodoListaEnlazada.h
        include/TablaHash.h
        include/ReporteGraficos.h
        include/NodoArbolB.h
        include/NodoArbolAVL.h
        include/NodoArbolBPlus.h
        include/CSVLoader.h
        include/Catalogo.h
        include/Producto.h
        include/Logger.h
        include/Benchmark.h
        include/Pila.h
)