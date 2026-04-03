//
// Created by mynorm50 on 7/3/26.
//

#ifndef ARBOL_B_PLUS_H
#define ARBOL_B_PLUS_H

#include <iostream>
#include <algorithm>
#include <string>
#include "NodoArbolBPlus.h"
#include "../Modelo/Producto.h"

class ArbolBPlus {
private:
    NodoArbolBPlus* raiz;

    // Utilidad para minsculas
    std::string toLower(std::string s) const {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    int comparar(const std::string& a, const std::string& b) const {
        std::string la = toLower(a);
        std::string lb = toLower(b);
        if (la < lb) return -1;
        if (la > lb) return  1;
        return 0;
    }

    // split especial de B+
    void split(NodoArbolBPlus* padre, int i, NodoArbolBPlus* hijoLleno) {
        NodoArbolBPlus* nuevoNodo = new NodoArbolBPlus(hijoLleno->getEsHoja());
        int medio = ORDEN_BP; // d = 2

        if (hijoLleno->getEsHoja()) {
            // split de hoja
            // en B+, la hoja derecha se queda con d+1 llaves (incluyendo la que sube)
            nuevoNodo->setNumLlaves(hijoLleno->getNumLlaves() - medio);
            for (int j = 0; j < nuevoNodo->getNumLlaves(); j++) {
                nuevoNodo->setLlave(j, hijoLleno->getLlave(j + medio));
            }
            hijoLleno->setNumLlaves(medio);

            // enlazar hojas (Lista Enlazada)
            nuevoNodo->setSiguiente(hijoLleno->getSiguiente());
            hijoLleno->setSiguiente(nuevoNodo);

            // subir copia de la primera llave del nuevo nodo al padre
            subirLlave(padre, i, nuevoNodo->getLlave(0), nuevoNodo);
        } else {
            // split de nodo interno
            // Igual que el arbol B normal
            nuevoNodo->setNumLlaves(hijoLleno->getNumLlaves() - medio - 1);
            for (int j = 0; j < nuevoNodo->getNumLlaves(); j++) {
                nuevoNodo->setLlave(j, hijoLleno->getLlave(j + medio + 1));
            }
            for (int j = 0; j <= nuevoNodo->getNumLlaves(); j++) {
                nuevoNodo->setHijo(j, hijoLleno->getHijo(j + medio + 1));
            }

            Producto llaveSubir = hijoLleno->getLlave(medio);
            hijoLleno->setNumLlaves(medio);
            subirLlave(padre, i, llaveSubir, nuevoNodo);
        }
    }

    void subirLlave(NodoArbolBPlus* padre, int i, const Producto& p, NodoArbolBPlus* nuevoHijo) {
        // Mover hijos a la derecha
        for (int j = padre->getNumLlaves(); j >= i + 1; j--) {
            padre->setHijo(j + 1, padre->getHijo(j));
        }
        padre->setHijo(i + 1, nuevoHijo);

        // Mover llaves a la derecha
        for (int j = padre->getNumLlaves() - 1; j >= i; j--) {
            padre->setLlave(j + 1, padre->getLlave(j));
        }
        padre->setLlave(i, p);
        padre->setNumLlaves(padre->getNumLlaves() + 1);
    }

    void insertarNoLleno(NodoArbolBPlus* nodo, const Producto& p) {
        int i = nodo->getNumLlaves() - 1;

        if (nodo->getEsHoja()) {
            // clave: categoria
            while (i >= 0 && comparar(p.categoria, nodo->getLlave(i).categoria) < 0) {
                nodo->setLlave(i + 1, nodo->getLlave(i));
                i--;
            }
            nodo->setLlave(i + 1, p);
            nodo->setNumLlaves(nodo->getNumLlaves() + 1);
        } else {
            // clave: categoria
            while (i >= 0 && comparar(p.categoria, nodo->getLlave(i).categoria) < 0) {
                i--;
            }
            i++;
            if (nodo->getHijo(i)->estaLleno()) {
                split(nodo, i, nodo->getHijo(i));
                if (comparar(p.categoria, nodo->getLlave(i).categoria) > 0) {
                    i++;
                }
            }
            insertarNoLleno(nodo->getHijo(i), p);
        }
    }

    // --- Eliminacion Arbol B+ ---

    // Buscar indice de la primera llave si es mayor o igaul a categoria
    int buscarIndice(NodoArbolBPlus* nodo, const std::string& categoria) {
        int idx = 0;
        while (idx < nodo->getNumLlaves() && comparar(nodo->getLlave(idx).categoria, categoria) < 0)
            idx++;
        return idx;
    }

    // Prestamo del hermano izquierdo (hoja)
    void prestarHojaAnterior(NodoArbolBPlus* padre, int idx) {
        NodoArbolBPlus* hijo   = padre->getHijo(idx);
        NodoArbolBPlus* hermano = padre->getHijo(idx - 1);

        // Mover llaves del hijo a la derecha
        for (int i = hijo->getNumLlaves(); i > 0; i--)
            hijo->setLlave(i, hijo->getLlave(i - 1));

        // La uiltima llave del hermano pasa al hijo
        hijo->setLlave(0, hermano->getLlave(hermano->getNumLlaves() - 1));
        hijo->setNumLlaves(hijo->getNumLlaves() + 1);
        hermano->setNumLlaves(hermano->getNumLlaves() - 1);

        // Actualizar índice en el padre
        padre->setLlave(idx - 1, hijo->getLlave(0));
    }

    // Prestamo del hermano derecho (hoja)
    void prestarHojaSiguiente(NodoArbolBPlus* padre, int idx) {
        NodoArbolBPlus* hijo    = padre->getHijo(idx);
        NodoArbolBPlus* hermano = padre->getHijo(idx + 1);

        // La primera llave del hermano pasa al hijo
        hijo->setLlave(hijo->getNumLlaves(), hermano->getLlave(0));
        hijo->setNumLlaves(hijo->getNumLlaves() + 1);

        // Mover llaves del hermano a la izquierda
        for (int i = 0; i < hermano->getNumLlaves() - 1; i++)
            hermano->setLlave(i, hermano->getLlave(i + 1));
        hermano->setNumLlaves(hermano->getNumLlaves() - 1);

        // Actualizar indice en el padre
        padre->setLlave(idx, hermano->getLlave(0));
    }

    // Fusion de dos hojas
    void fusionarHojas(NodoArbolBPlus* padre, int idx) {
        NodoArbolBPlus* hijo    = padre->getHijo(idx);
        NodoArbolBPlus* hermano = padre->getHijo(idx + 1);

        // Copiar llaves del hermano al hijo
        for (int i = 0; i < hermano->getNumLlaves(); i++)
            hijo->setLlave(hijo->getNumLlaves() + i, hermano->getLlave(i));
        hijo->setNumLlaves(hijo->getNumLlaves() + hermano->getNumLlaves());

        // Mantener enlace de hojas
        hijo->setSiguiente(hermano->getSiguiente());

        // Eliminar llave del padre
        for (int i = idx + 1; i < padre->getNumLlaves(); i++)
            padre->setLlave(i - 1, padre->getLlave(i));
        for (int i = idx + 2; i <= padre->getNumLlaves(); i++)
            padre->setHijo(i - 1, padre->getHijo(i));
        padre->setNumLlaves(padre->getNumLlaves() - 1);

        delete hermano;
    }

    // prestamo del hermano izquierdo (nodo interno)
    void prestarInternoAnterior(NodoArbolBPlus* padre, int idx) {
        NodoArbolBPlus* hijo    = padre->getHijo(idx);
        NodoArbolBPlus* hermano = padre->getHijo(idx - 1);

        // Mover llaves e hijos del hijo a la derecha
        for (int i = hijo->getNumLlaves(); i > 0; i--)
            hijo->setLlave(i, hijo->getLlave(i - 1));
        for (int i = hijo->getNumLlaves() + 1; i > 0; i--)
            hijo->setHijo(i, hijo->getHijo(i - 1));

        // Bajar llave del padre al hijo
        hijo->setLlave(0, padre->getLlave(idx - 1));
        hijo->setHijo(0, hermano->getHijo(hermano->getNumLlaves()));
        hijo->setNumLlaves(hijo->getNumLlaves() + 1);

        // Subir ultima llave del hermano al padre
        padre->setLlave(idx - 1, hermano->getLlave(hermano->getNumLlaves() - 1));
        hermano->setNumLlaves(hermano->getNumLlaves() - 1);
    }

    // Prestamo del hermano derecho (nodo interno)
    void prestarInternoSiguiente(NodoArbolBPlus* padre, int idx) {
        NodoArbolBPlus* hijo    = padre->getHijo(idx);
        NodoArbolBPlus* hermano = padre->getHijo(idx + 1);

        // Bajar llave del padre al hijo
        hijo->setLlave(hijo->getNumLlaves(), padre->getLlave(idx));
        hijo->setHijo(hijo->getNumLlaves() + 1, hermano->getHijo(0));
        hijo->setNumLlaves(hijo->getNumLlaves() + 1);

        // Subir primera llave del hermano al padre
        padre->setLlave(idx, hermano->getLlave(0));

        // Mover llaves e hijos del hermano a la izquierda
        for (int i = 0; i < hermano->getNumLlaves() - 1; i++)
            hermano->setLlave(i, hermano->getLlave(i + 1));
        for (int i = 0; i < hermano->getNumLlaves(); i++)
            hermano->setHijo(i, hermano->getHijo(i + 1));
        hermano->setNumLlaves(hermano->getNumLlaves() - 1);
    }

    // Fusiion de nodos internos
    void fusionarInternos(NodoArbolBPlus* padre, int idx) {
        NodoArbolBPlus* hijo    = padre->getHijo(idx);
        NodoArbolBPlus* hermano = padre->getHijo(idx + 1);

        // Bajar llave del padre al hijo
        hijo->setLlave(hijo->getNumLlaves(), padre->getLlave(idx));
        hijo->setNumLlaves(hijo->getNumLlaves() + 1);

        // Copiar llaves e hijos del hermano al hijo
        for (int i = 0; i < hermano->getNumLlaves(); i++)
            hijo->setLlave(hijo->getNumLlaves() + i, hermano->getLlave(i));
        for (int i = 0; i <= hermano->getNumLlaves(); i++)
            hijo->setHijo(hijo->getNumLlaves() + i, hermano->getHijo(i));
        hijo->setNumLlaves(hijo->getNumLlaves() + hermano->getNumLlaves());

        // Eliminar llave del padre
        for (int i = idx + 1; i < padre->getNumLlaves(); i++)
            padre->setLlave(i - 1, padre->getLlave(i));
        for (int i = idx + 2; i <= padre->getNumLlaves(); i++)
            padre->setHijo(i - 1, padre->getHijo(i));
        padre->setNumLlaves(padre->getNumLlaves() - 1);

        delete hermano;
    }

    // logica recursiva principal
    bool eliminarRecursivo(NodoArbolBPlus* nodo, const std::string& categoria, const std::string& codigo) {
    if (nodo->getEsHoja()) {
        for (int i = 0; i < nodo->getNumLlaves(); i++) {
            if (comparar(nodo->getLlave(i).categoria, categoria) == 0 &&
                nodo->getLlave(i).codigoBarras == codigo) {
                for (int j = i + 1; j < nodo->getNumLlaves(); j++)
                    nodo->setLlave(j - 1, nodo->getLlave(j));
                nodo->setNumLlaves(nodo->getNumLlaves() - 1);
                return true;
            }
        }
        return false;
    }

    // Nodo interno: buscar el hijo correcto
    int idx = 0;
    while (idx < nodo->getNumLlaves() &&
        comparar(categoria, nodo->getLlave(idx).categoria) > 0) {
        idx++;
    }
    //si es igual, tambien puede estar en el hijo izquierdo (idx sin incrementar)

    NodoArbolBPlus* hijo = nodo->getHijo(idx);
    bool eliminado = eliminarRecursivo(hijo, categoria, codigo);

    // Si no encontr en hijo izquierdo y la categoria coincide con el indice, probar derecho
    if (!eliminado && idx < nodo->getNumLlaves() &&
        comparar(categoria, nodo->getLlave(idx).categoria) == 0) {
        hijo = nodo->getHijo(idx + 1);
        eliminado = eliminarRecursivo(hijo, categoria, codigo);
        idx = idx + 1;
    }

    if (!eliminado) return false;

    // Actualizar indice si la primera llave del hijo cambió
    if (idx > 0 && hijo->getNumLlaves() > 0)
        nodo->setLlave(idx - 1, hijo->getLlave(0));

    // Verificar underflow
    if (hijo->getNumLlaves() < ORDEN_BP) {
        if (hijo->getEsHoja()) {
            if (idx > 0 && nodo->getHijo(idx - 1)->getNumLlaves() > ORDEN_BP)
                prestarHojaAnterior(nodo, idx);
            else if (idx < nodo->getNumLlaves() && nodo->getHijo(idx + 1)->getNumLlaves() > ORDEN_BP)
                prestarInternoSiguiente(nodo, idx);
            else if (idx < nodo->getNumLlaves())
                fusionarHojas(nodo, idx);
            else
                fusionarHojas(nodo, idx - 1);
        } else {
            if (idx > 0 && nodo->getHijo(idx - 1)->getNumLlaves() > ORDEN_BP)
                prestarInternoAnterior(nodo, idx);
            else if (idx < nodo->getNumLlaves() && nodo->getHijo(idx + 1)->getNumLlaves() > ORDEN_BP)
                prestarInternoSiguiente(nodo, idx);
            else if (idx < nodo->getNumLlaves())
                fusionarInternos(nodo, idx);
            else
                fusionarInternos(nodo, idx - 1);
        }
    }
    return true;
}

public:
    ArbolBPlus() : raiz(nullptr) {}

    NodoArbolBPlus* getRaiz() { return raiz; }

    void insertar(const Producto& p) {
        if (raiz == nullptr) {
            raiz = new NodoArbolBPlus(true);
            raiz->setLlave(0, p);
            raiz->setNumLlaves(1);
            return;
        }

        if (raiz->estaLleno()) {
            NodoArbolBPlus* nuevaRaiz = new NodoArbolBPlus(false);
            nuevaRaiz->setHijo(0, raiz);
            split(nuevaRaiz, 0, raiz);
            raiz = nuevaRaiz;
        }
        insertarNoLleno(raiz, p);
    }

    // recorrido por hojas (Lineal)
    void imprimirLineal() const {
        if (raiz == nullptr) return;

        // 1. Bajar hasta la primera hoja (la mas a la izquierda)
        NodoArbolBPlus* actual = raiz;
        while (!actual->getEsHoja()) {
            actual = actual->getHijo(0);
        }

        // 2. Recorrer usando el puntero 'siguiente'
        while (actual != nullptr) {
            for (int i = 0; i < actual->getNumLlaves(); i++) {
                std::cout << actual->getLlave(i).nombre
                    << " | " << actual->getLlave(i).categoria << " [B+ Leaf]\n";
            }
            actual = actual->getSiguiente();
        }
    }

    // busqueda por categoría (recorrido eficiente por hojas)
    void buscarPorCategoria(const std::string& categoria) const {
        if (raiz == nullptr) return;

        // 1. Bajar hasta la PRIMERA hoja que podría contener la categoría
        NodoArbolBPlus* actual = raiz;
        while (!actual->getEsHoja()) {
            int i = 0;
            // En B+, bajamos por el hijo i si la categoria es menor o igual a llave[i]
            while (i < actual->getNumLlaves() &&
                comparar(categoria, actual->getLlave(i).categoria) > 0) {
                i++; }
            actual = actual->getHijo(i);
        }

        // 2. Recorrer linealmente las hojas usando el puntero 'siguiente'
        std::cout << "Productos en categoria '" << categoria << "':\n";
        bool encontrada = false;
        while (actual != nullptr) {
            for (int i = 0; i < actual->getNumLlaves(); i++) {
                int comp = comparar(actual->getLlave(i).categoria, categoria);
                if (comp == 0) {
                    std::cout << "- " << actual->getLlave(i).nombre
                        << " [" << actual->getLlave(i).codigoBarras << "]\n";
                    encontrada = true;
                } else if (comp > 0) {
                    // Como están ordenadas, si ya pasamos la categoría, terminamos
                    if (!encontrada) std::cout << "(No se encontraron productos)\n";
                    return;
                }
            }
            actual = actual->getSiguiente();
        }
        if (!encontrada) std::cout << "(No se encontraron productos)\n";
    }

    // busqueda puntual por categoria (devuelve primer producto encontrado)
    Producto* buscar(const std::string& categoria) {
        if (raiz == nullptr) return nullptr;
        NodoArbolBPlus* actual = raiz;

        // Bajar hasta la hoja
        while (!actual->getEsHoja()) {
            int i = 0;
            while (i < actual->getNumLlaves() &&
                comparar(categoria, actual->getLlave(i).categoria) >= 0) {
                i++;
            }
            actual = actual->getHijo(i);
        }

        // Buscar en la hoja
        for (int i = 0; i < actual->getNumLlaves(); i++) {
            if (comparar(categoria, actual->getLlave(i).categoria) == 0) {
                return &(actual->getLlave(i));
            }
        }
        return nullptr;
    }

    bool eliminar(const std::string& categoria, const std::string& codigo) {
        if (raiz == nullptr) return false;

        bool eliminado = eliminarRecursivo(raiz, categoria, codigo);

        // Si la raiz quedó vacaa, el arbol baja un nivel
        if (raiz->getNumLlaves() == 0) {
            NodoArbolBPlus* viejaRaiz = raiz;
            if (raiz->getEsHoja()) raiz = nullptr;
            else raiz = raiz->getHijo(0);
            delete viejaRaiz;
        }
        return eliminado;
    }
};

#endif