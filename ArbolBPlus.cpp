#include "ArbolBPlus.h"
#include <algorithm>
#include <iostream>

using namespace std;

NodoBPlusBase::NodoBPlusBase(int orden, bool es_hoja) {
    this->orden = orden;       
    this->es_hoja = es_hoja;    
}

NodoBPlusBase::~NodoBPlusBase() {}

bool NodoBPlusBase::es_hoja_nodo() const {
    return es_hoja;
}

int NodoBPlusBase::get_orden() const {
    return orden;
}

vector<int> NodoBPlusBase::get_claves() const {
    return claves;
}

int NodoBPlusBase::num_claves() const {
    return claves.size();
}

void NodoBPlusBase::set_clave(int index, int valor) {
    if (index >= 0 && index < (int)claves.size()) {
        claves[index] = valor;
    }
}

void NodoBPlusBase::agregar_clave(int clave) {
    claves.push_back(clave);
}

void NodoBPlusBase::eliminar_clave_en(int index) {
    if (index >= 0 && index < (int)claves.size()) {
        claves.erase(claves.begin() + index);
    }
}

// NodoB Hoja
NodoBHoja::NodoBHoja(int orden) 
    : NodoBPlusBase(orden, true), siguiente_hoja(nullptr) {}

NodoBHoja::~NodoBHoja() {}

void NodoBHoja::insertar(int clave, NodoGrafo* dato) {
    auto it = lower_bound(claves.begin(), claves.end(), clave);
    int pos = it - claves.begin();
    claves.insert(it, clave);
    datos.insert(datos.begin() + pos, dato);
}

NodoGrafo* NodoBHoja::buscar(int clave) {
    auto it = find(claves.begin(), claves.end(), clave);
    if (it != claves.end()) {
        int pos = it - claves.begin();
        return datos[pos];
    }
    return nullptr;
}

NodoBHoja* NodoBHoja::dividir() {
    int mid = claves.size() / 2;
    NodoBHoja* nueva_hoja = new NodoBHoja(orden);
    
    nueva_hoja->claves.assign(claves.begin() + mid, claves.end());
    nueva_hoja->datos.assign(datos.begin() + mid, datos.end());
    
    claves.erase(claves.begin() + mid, claves.end());
    datos.erase(datos.begin() + mid, datos.end());
    
    nueva_hoja->siguiente_hoja = this->siguiente_hoja;
    this->siguiente_hoja = nueva_hoja;
    
    return nueva_hoja;
}

void NodoBHoja::set_siguiente(NodoBHoja* sig) {
    siguiente_hoja = sig;
}

NodoBHoja* NodoBHoja::get_siguiente() const {
    return siguiente_hoja;
}

vector<NodoGrafo*> NodoBHoja::get_datos() const {
    return datos;
}

bool NodoBHoja::eliminar(int clave) {
    auto it = find(claves.begin(), claves.end(), clave);
    if (it != claves.end()) {
        int pos = it - claves.begin();
        claves.erase(it);
        datos.erase(datos.begin() + pos);
        return true;
    }
    return false;
}

// NodoB Interno
NodoBInterno::NodoBInterno(int orden) 
    : NodoBPlusBase(orden, false) {
    punteros.push_back(nullptr);
}

NodoBInterno::~NodoBInterno() {}

int NodoBInterno::buscar_siguiente(int clave) {
    int pos = 0;
    while (pos < (int)claves.size() && clave >= claves[pos]) {
        pos++;
    }
    return pos;
}

void NodoBInterno::insertar_clave(int clave, NodoBPlusBase* puntero) {
    auto it = lower_bound(claves.begin(), claves.end(), clave);
    int pos = it - claves.begin();
    claves.insert(it, clave);
    punteros.insert(punteros.begin() + pos + 1, puntero);
}

NodoBInterno* NodoBInterno::dividir(int& clave_media) {
    int mid = claves.size() / 2;
    clave_media = claves[mid];
    
    NodoBInterno* nuevo_interno = new NodoBInterno(orden);
    nuevo_interno->punteros.clear();
    
    nuevo_interno->claves.assign(claves.begin() + mid + 1, claves.end());
    nuevo_interno->punteros.assign(punteros.begin() + mid + 1, punteros.end());
    
    claves.erase(claves.begin() + mid, claves.end());
    punteros.erase(punteros.begin() + mid + 1, punteros.end());
    
    return nuevo_interno;
}

NodoBPlusBase* NodoBInterno::get_puntero(int indice) {
    if (indice >= 0 && indice < (int)punteros.size()) {
        return punteros[indice];
    }
    return nullptr;
}

int NodoBInterno::num_punteros() const {
    return punteros.size();
}

void NodoBInterno::set_puntero(int index, NodoBPlusBase* puntero) {
    if (index >= 0 && index < (int)punteros.size()) {
        punteros[index] = puntero;
    }
}

void NodoBInterno::agregar_puntero(NodoBPlusBase* puntero) {
    punteros.push_back(puntero);
}

// ArbolBPlus
ArbolBPlus::ArbolBPlus(int orden) : orden(orden), accesos_disco(0) {
    raiz = new NodoBHoja(orden);
}

ArbolBPlus::~ArbolBPlus() {
    destruir_arbol(raiz);
}

void ArbolBPlus::destruir_arbol(NodoBPlusBase* nodo) {
    if (!nodo) return;
    
    if (!nodo->es_hoja_nodo()) {
        NodoBInterno* interno = dynamic_cast<NodoBInterno*>(nodo);
        for (int i = 0; i < interno->num_punteros(); i++) {
            destruir_arbol(interno->get_puntero(i));
        }
    }
    delete nodo;
}

void ArbolBPlus::insertar(int clave, NodoGrafo* dato) {
    int clave_promovida = -1;
    NodoBPlusBase* nuevo_nodo = nullptr;
    
    insertar_recursivo(raiz, clave, dato, clave_promovida, nuevo_nodo);
    
    if (nuevo_nodo != nullptr) {
        NodoBInterno* nueva_raiz = new NodoBInterno(orden);
        nueva_raiz->set_puntero(0, raiz);
        nueva_raiz->insertar_clave(clave_promovida, nuevo_nodo);
        raiz = nueva_raiz;
    }
}

void ArbolBPlus::insertar_recursivo(NodoBPlusBase* nodo, int clave, NodoGrafo* dato,
                                    int& clave_promovida, NodoBPlusBase*& nuevo_nodo) {
    accesos_disco++;
    
    if (nodo->es_hoja_nodo()) {
        NodoBHoja* hoja = dynamic_cast<NodoBHoja*>(nodo);
        hoja->insertar(clave, dato);
        
        if (hoja->num_claves() >= orden) {
            NodoBHoja* nueva_hoja = hoja->dividir();
            clave_promovida = nueva_hoja->get_claves()[0];
            nuevo_nodo = nueva_hoja;
        } else {
            nuevo_nodo = nullptr;
        }
    } else {
        NodoBInterno* interno = dynamic_cast<NodoBInterno*>(nodo);
        int pos = interno->buscar_siguiente(clave);
        
        int clave_prom_hijo = -1;
        NodoBPlusBase* nuevo_hijo = nullptr;
        
        insertar_recursivo(interno->get_puntero(pos), clave, dato, 
                          clave_prom_hijo, nuevo_hijo);
        
        if (nuevo_hijo != nullptr) {
            interno->insertar_clave(clave_prom_hijo, nuevo_hijo);
            
            if (interno->num_claves() >= orden) {
                int clave_media;
                NodoBInterno* nuevo_interno = interno->dividir(clave_media);
                clave_promovida = clave_media;
                nuevo_nodo = nuevo_interno;
            } else {
                nuevo_nodo = nullptr;
            }
        } else {
            nuevo_nodo = nullptr;
        }
    }
}

NodoGrafo* ArbolBPlus::buscar(int clave) {
    accesos_disco = 0;
    NodoBPlusBase* actual = raiz;
    
    while (actual != nullptr) {
        accesos_disco++;
        
        if (actual->es_hoja_nodo()) {
            NodoBHoja* hoja = dynamic_cast<NodoBHoja*>(actual);
            return hoja->buscar(clave);
        } else {
            NodoBInterno* interno = dynamic_cast<NodoBInterno*>(actual);
            int pos = interno->buscar_siguiente(clave);
            actual = interno->get_puntero(pos);
        }
    }
    
    return nullptr;
}

bool ArbolBPlus::eliminar(int clave) {
    return eliminar_recursivo(raiz, clave);
}

bool ArbolBPlus::eliminar_recursivo(NodoBPlusBase* nodo, int clave) {
    if (!nodo) return false;
    
    accesos_disco++;
    
    if (nodo->es_hoja_nodo()) {
        NodoBHoja* hoja = dynamic_cast<NodoBHoja*>(nodo);
        return hoja->eliminar(clave);
    } else {
        NodoBInterno* interno = dynamic_cast<NodoBInterno*>(nodo);
        int pos = interno->buscar_siguiente(clave);
        return eliminar_recursivo(interno->get_puntero(pos), clave);
    }
}

int ArbolBPlus::get_accesos_disco() const {
    return accesos_disco;
}

void ArbolBPlus::resetear_accesos() {
    accesos_disco = 0;
}