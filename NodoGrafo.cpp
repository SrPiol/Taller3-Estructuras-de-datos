#include "NodoGrafo.h"
#include <algorithm>

using namespace std;

// NodoGrafo
NodoGrafo::NodoGrafo(int id) : id(id) {}

NodoGrafo::~NodoGrafo() {}

int NodoGrafo::get_id() const {
    return id;
}

vector<int> NodoGrafo::lista_padres() const {
    return padres;
}

void NodoGrafo::agregar_padre(int id_padre) {
    if (find(padres.begin(), padres.end(), id_padre) == padres.end()) {
        padres.push_back(id_padre);
    }
}

void NodoGrafo::eliminar_padre(int id_padre) {
    padres.erase(remove(padres.begin(), padres.end(), id_padre), padres.end());
}

// NodoDirectorio
NodoDirectorio::NodoDirectorio(int id) : NodoGrafo(id) {}

bool NodoDirectorio::es_directorio() const {
    return true;
}

vector<int> NodoDirectorio::lista_hijos() const {
    return hijos;
}

void NodoDirectorio::agregar_hijo(int id_hijo) {
    if (find(hijos.begin(), hijos.end(), id_hijo) == hijos.end()) {
        hijos.push_back(id_hijo);
    }
}

void NodoDirectorio::eliminar_hijo(int id_hijo) {
    hijos.erase(remove(hijos.begin(), hijos.end(), id_hijo), hijos.end());
}

// NodoArchivo
NodoArchivo::NodoArchivo(int id, int tamanio, int tipo) 
    : NodoGrafo(id), tamanio(tamanio), tipo(tipo) {}

bool NodoArchivo::es_directorio() const {
    return false;
}

int NodoArchivo::get_tamanio() const {
    return tamanio;
}

int NodoArchivo::get_tipo() const {
    return tipo;
}