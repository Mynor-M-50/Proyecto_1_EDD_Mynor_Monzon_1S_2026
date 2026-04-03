//
// Created by mynorm50 on 7/3/26.
//

#ifndef ARBOL_B_H
#define ARBOL_B_H

#include <iostream>
#include <algorithm>
#include <cctype>
#include <string>
#include "NodoArbolB.h"
#include "Producto.h"

class ArbolB {
private:
    NodoArbolB* raiz;

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

    // split
    void split(NodoArbolB* padre, int i, NodoArbolB* hijoLleno) {
        NodoArbolB* nuevaPagina = new NodoArbolB(hijoLleno->getEsHoja());

        int medio = ORDEN_B; // = 2

        // paggina que obtiene las llaves de la derecha
        int j;
        for (j = 0; j < hijoLleno->getNumLlaves() - medio - 1; j++) {
            nuevaPagina->setLlave(j, hijoLleno->getLlave(j + medio + 1));
        }

        nuevaPagina->setNumLlaves(hijoLleno->getNumLlaves() - medio - 1);

        // si no es hoja copiar hijops
        if (!hijoLleno->getEsHoja()) {
            for (j = 0; j < nuevaPagina->getNumLlaves() + 1; j++) {
                nuevaPagina->setHijo(j, hijoLleno->getHijo(j + medio + 1));
            }
        }

        // reducir hijo izq
        hijoLleno->setNumLlaves(medio);

        // mover hijos a padre
        for (j = padre->getNumLlaves(); j >= i + 1; j--) {
            padre->setHijo(j + 1, padre->getHijo(j));
        }

        padre->setHijo(i + 1, nuevaPagina);

        // mover llaves de padre
        for (j = padre->getNumLlaves() - 1; j >= i; j--) {
            padre->setLlave(j + 1, padre->getLlave(j));
        }

        // subir lllave central
        padre->setLlave(i, hijoLleno->getLlave(medio));
        padre->setNumLlaves(padre->getNumLlaves() + 1);
    }

    // insertar en nodo que no esta lleno
    void insertarNoLleno(NodoArbolB* nodo, const Producto& p) {
        int i = nodo->getNumLlaves() - 1;

        if (nodo->getEsHoja()) {
            while (i >= 0 && comparar(p.fechaVencimiento, nodo->getLlave(i).fechaVencimiento) < 0) {
                nodo->setLlave(i + 1, nodo->getLlave(i));
                i--;
            }
            nodo->setLlave(i + 1, p);
            nodo->setNumLlaves(nodo->getNumLlaves() + 1);
        } else {
            while (i >= 0 && comparar(p.fechaVencimiento, nodo->getLlave(i).fechaVencimiento) < 0) {
                i--;
            }
            i++;

            if (nodo->getHijo(i)->estaLleno()) {
                split(nodo, i, nodo->getHijo(i));
                if (comparar(p.fechaVencimiento, nodo->getLlave(i).fechaVencimiento) > 0) {
                    i++;
                }
            }
            insertarNoLleno(nodo->getHijo(i), p);
        }
    }

    // Buscar
    Producto* buscar(NodoArbolB* nodo, const std::string& fecha) {
        if (nodo == nullptr) return nullptr;

        int i = 0;
        while (i < nodo->getNumLlaves() &&
            comparar(fecha, nodo->getLlave(i).fechaVencimiento) > 0) {
            i++;
            }

        if (i < nodo->getNumLlaves() &&
            comparar(fecha, nodo->getLlave(i).fechaVencimiento) == 0) {
            return &(nodo->getLlave(i));
            }

        if (nodo->getEsHoja()) return nullptr;

        return buscar(nodo->getHijo(i), fecha);
    }

    // Destructor
    void destruir(NodoArbolB* nodo) {
        if (nodo == nullptr) return;
        if (!nodo->getEsHoja()) {
            for (int i = 0; i <= nodo->getNumLlaves(); i++) {
                destruir(nodo->getHijo(i));
            }
        }
        delete nodo;
    }

    // Inorden
    void inorden(NodoArbolB* nodo) const {
        if (nodo == nullptr) return;

        int i;
        for (i = 0; i < nodo->getNumLlaves(); i++) {
            // Si no es hoja, primero visita el hijo izquierdo de la llave i
            if (!nodo->getEsHoja()) {
                inorden(nodo->getHijo(i));
            }
            // Imprime la llave i
            std::cout << nodo->getLlave(i).nombre << " | " << nodo->getLlave(i).fechaVencimiento << " [B-Tree]\n";
        }

        // Al terminar el ciclo, falta visitar el ultimo hijo (el de la derecha de la ultima llav
        if (!nodo->getEsHoja()) {
            inorden(nodo->getHijo(i));
        }
    }

    // privado
    void buscarRango(NodoArbolB* nodo, const std::string& desde, const std::string& hasta) const {
        if (nodo == nullptr) return;

        int i;
        for (i = 0; i < nodo->getNumLlaves(); i++) {
            // Primero visitar hijo izquierdo (inorden)
            if (!nodo->getEsHoja()) {
                buscarRango(nodo->getHijo(i), desde, hasta);
            }

            // Luego revisar si la llave esta en rango
            const std::string& f = nodo->getLlave(i).fechaVencimiento;
            if (comparar(f, desde) >= 0 && comparar(f, hasta) <= 0) {
                std::cout << nodo->getLlave(i).nombre
                << " | " << f
                << " | $" << nodo->getLlave(i).precio << "\n";
            }
        }

        // visitar ultimo hijo
        if (!nodo->getEsHoja()) {
            buscarRango(nodo->getHijo(i), desde, hasta);
        }
    }

    // --- Metodos de eliminacion para Arbol B ---

    // 1. Buscar el indice de la llave en el nodo
    int buscarIndice(NodoArbolB* nodo, const std::string& fecha) {
        int idx = 0;
        while (idx < nodo->getNumLlaves() && comparar(nodo->getLlave(idx).fechaVencimiento, fecha) < 0) {
            idx++;
        }
        return idx;
    }

    // 2. obtener el predecesor (el mas grande del subarbol izquierdo)
    Producto getPredecesor(NodoArbolB* nodo, int idx) {
        NodoArbolB* actual = nodo->getHijo(idx);
        while (!actual->getEsHoja()) {
            actual = actual->getHijo(actual->getNumLlaves());
        }
        return actual->getLlave(actual->getNumLlaves() - 1);
    }

    // 3. Obtener el sucesor (el mas pequeño del subáarbol derecho)
    Producto getSucesor(NodoArbolB* nodo, int idx) {
        NodoArbolB* actual = nodo->getHijo(idx + 1);
        while (!actual->getEsHoja()) {
            actual = actual->getHijo(0);
        }
        return actual->getLlave(0);
    }

    // 4. Prstamo del hermano izquierdo
    void prestarDeAnterior(NodoArbolB* nodo, int idx) {
        NodoArbolB* hijo = nodo->getHijo(idx);
        NodoArbolB* hermano = nodo->getHijo(idx - 1);

        // Mover todas las llaves del hijo un lugar a la derecha
        for (int i = hijo->getNumLlaves() - 1; i >= 0; --i)
            hijo->setLlave(i + 1, hijo->getLlave(i));

        // Si no es hoja, mover tambin los hijos
        if (!hijo->getEsHoja()) {
            for (int i = hijo->getNumLlaves(); i >= 0; --i)
                hijo->setHijo(i + 1, hijo->getHijo(i));
        }

        // La primera llave del hijo ahora es la llave del padre
        hijo->setLlave(0, nodo->getLlave(idx - 1));

        // El primer hijo del hijo ahora es el ultimo hijo del hermano
        if (!hijo->getEsHoja())
            hijo->setHijo(0, hermano->getHijo(hermano->getNumLlaves()));

        // La llave del padre ahora es la ultima llave del hermano
        nodo->setLlave(idx - 1, hermano->getLlave(hermano->getNumLlaves() - 1));

        hijo->setNumLlaves(hijo->getNumLlaves() + 1);
        hermano->setNumLlaves(hermano->getNumLlaves() - 1);
    }

    // 5. Prsstamo del hermano derecho
    void prestarDeSiguiente(NodoArbolB* nodo, int idx) {
        NodoArbolB* hijo = nodo->getHijo(idx);
        NodoArbolB* hermano = nodo->getHijo(idx + 1);

        // La utima llave del hijo es la llave del padre
        hijo->setLlave(hijo->getNumLlaves(), nodo->getLlave(idx));

        // El ultimo hijo del hijo es el primer hijo del hermano
        if (!hijo->getEsHoja())
            hijo->setHijo(hijo->getNumLlaves() + 1, hermano->getHijo(0));

        // La llave del padre es la primera llave del hermano
        nodo->setLlave(idx, hermano->getLlave(0));

        // Mover llaves del hermano a la izquierda
        for (int i = 1; i < hermano->getNumLlaves(); ++i)
            hermano->setLlave(i - 1, hermano->getLlave(i));

        // Mover hijos del hermano a la izquierda
        if (!hermano->getEsHoja()) {
            for (int i = 1; i <= hermano->getNumLlaves(); ++i)
                hermano->setHijo(i - 1, hermano->getHijo(i));
        }

        hijo->setNumLlaves(hijo->getNumLlaves() + 1);
        hermano->setNumLlaves(hermano->getNumLlaves() - 1);
    }

    // 6. Fusin de dos nodos
    void fusionar(NodoArbolB* nodo, int idx) {
        NodoArbolB* hijo = nodo->getHijo(idx);
        NodoArbolB* hermano = nodo->getHijo(idx + 1);

        // Bajar la llave del padre al hijo
        hijo->setLlave(ORDEN_B, nodo->getLlave(idx));

        // Copiar llaves del hermano al hijo
        for (int i = 0; i < hermano->getNumLlaves(); ++i)
            hijo->setLlave(i + ORDEN_B + 1, hermano->getLlave(i));

        // Copiar hijos del hermano al hijo
        if (!hijo->getEsHoja()) {
            for (int i = 0; i <= hermano->getNumLlaves(); ++i)
                hijo->setHijo(i + ORDEN_B + 1, hermano->getHijo(i));
        }

        // Mover llaves del padre a la izquierda para llenar el hueco
        for (int i = idx + 1; i < nodo->getNumLlaves(); ++i)
            nodo->setLlave(i - 1, nodo->getLlave(i));

        // Mover hijos del padre a la izquierda
        for (int i = idx + 2; i <= nodo->getNumLlaves(); ++i)
            nodo->setHijo(i - 1, nodo->getHijo(i));

        hijo->setNumLlaves(hijo->getNumLlaves() + hermano->getNumLlaves() + 1);
        nodo->setNumLlaves(nodo->getNumLlaves() - 1);

        delete hermano;
    }

    // 7. logica principal de eliminacin recursiva
    void eliminarRecursivo(NodoArbolB* nodo, const std::string& fecha) {
        int idx = buscarIndice(nodo, fecha);

        // Caso A: La llave esta en este nodo
        if (idx < nodo->getNumLlaves() && comparar(nodo->getLlave(idx).fechaVencimiento, fecha) == 0) {
            if (nodo->getEsHoja()) {
                // Casi A1: Es hoja, solo borrar y mover el resto
                for (int i = idx + 1; i < nodo->getNumLlaves(); ++i)
                    nodo->setLlave(i - 1, nodo->getLlave(i));
                nodo->setNumLlaves(nodo->getNumLlaves() - 1);
            } else {
                // caso A2: Nodo interno, reemplazar por predecesor o sucesor
                if (nodo->getHijo(idx)->getNumLlaves() >= ORDEN_B) {
                    Producto pred = getPredecesor(nodo, idx);
                    nodo->setLlave(idx, pred);
                    eliminarRecursivo(nodo->getHijo(idx), pred.fechaVencimiento);
                } else if (nodo->getHijo(idx + 1)->getNumLlaves() >= ORDEN_B) {
                    Producto suc = getSucesor(nodo, idx);
                    nodo->setLlave(idx, suc);
                    eliminarRecursivo(nodo->getHijo(idx + 1), suc.fechaVencimiento);
                } else {
                    // Ambos hijos tienen d-1 llaves, fusionar
                    fusionar(nodo, idx);
                    eliminarRecursivo(nodo->getHijo(idx), fecha);
                }
            }
        } else {
            // caso B: La llave no está aqui, buscar en el hijo
            if (nodo->getEsHoja()) return; // No existe

            bool esUltimoHijo = (idx == nodo->getNumLlaves());

            // Si el hijo donde deberi estar tiene pocas llaves, balancear antes de bajar
            if (nodo->getHijo(idx)->getNumLlaves() < ORDEN_B) {
                if (idx != 0 && nodo->getHijo(idx - 1)->getNumLlaves() >= ORDEN_B)
                    prestarDeAnterior(nodo, idx);
                else if (idx != nodo->getNumLlaves() && nodo->getHijo(idx + 1)->getNumLlaves() >= ORDEN_B)
                    prestarDeSiguiente(nodo, idx);
                else {
                    if (idx != nodo->getNumLlaves())
                        fusionar(nodo, idx);
                    else
                        fusionar(nodo, idx - 1);
                }
            }

            // deaspues del balanceo, el hijo pudo haber cambiado
            if (esUltimoHijo && idx > nodo->getNumLlaves())
                eliminarRecursivo(nodo->getHijo(idx - 1), fecha);
            else
                eliminarRecursivo(nodo->getHijo(idx), fecha);
        }
    }

public:
    ArbolB() : raiz(nullptr) {}

    ~ArbolB() { destruir(raiz); }

    bool insertar(const Producto& p) {
        if (raiz == nullptr) {
            raiz = new NodoArbolB(true);
            raiz->setLlave(0, p);
            raiz->setNumLlaves(1);
            return true;
        }

        if (raiz->estaLleno()) {
            NodoArbolB* nuevaRaiz = new NodoArbolB(false);
            nuevaRaiz->setHijo(0, raiz);
            split(nuevaRaiz, 0, raiz);
            raiz = nuevaRaiz;
        }

        insertarNoLleno(raiz, p);
        return true;
    }

    Producto* buscar(const std::string& fecha) {
        return buscar(raiz, fecha);
    }

    bool isEmpty() const { return raiz == nullptr; }
    NodoArbolB* getRaiz() const { return raiz; }

    void imprimirInorden() const { inorden(raiz); }

    void buscarPorRango(const std::string& desde, const std::string& hasta) const {
        std::cout << "Productos con vencimiento entre " << desde << " y " << hasta << ":\n";
        buscarRango(raiz, desde, hasta);
    }

    bool eliminar(const std::string& fecha) {
        if (raiz == nullptr) return false;

        eliminarRecursivo(raiz, fecha);

        // Si la raiz quedo vacía después de una fudion, el arbol baja un nivel
        if (raiz->getNumLlaves() == 0) {
            NodoArbolB* viejaRaiz = raiz;
            if (raiz->getEsHoja()) raiz = nullptr;
            else raiz = raiz->getHijo(0);
            delete viejaRaiz;
        }
        return true;
    }
};

#endif