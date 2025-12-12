#ifndef ARBOLBPLUS_H
#define ARBOLBPLUS_H

#include "NodoGrafo.h"
#include <vector>

using namespace std;

class NodoBPlusBase {
protected:
    vector<int> claves;
    int orden;
    bool es_hoja;

public:
    NodoBPlusBase(int orden, bool es_hoja);
    virtual ~NodoBPlusBase();
    
    bool es_hoja_nodo() const;
    int get_orden() const;
    vector<int> get_claves() const;
    int num_claves() const;
    void set_clave(int index, int valor);
    void agregar_clave(int clave);
    void eliminar_clave_en(int index);
};

class NodoBHoja : public NodoBPlusBase {
private:
    vector<NodoGrafo*> datos;
    NodoBHoja* siguiente_hoja;

public:
    NodoBHoja(int orden);
    ~NodoBHoja();
    
    void insertar(int clave, NodoGrafo* dato);
    NodoGrafo* buscar(int clave);
    NodoBHoja* dividir();
    void set_siguiente(NodoBHoja* sig);
    NodoBHoja* get_siguiente() const;
    vector<NodoGrafo*> get_datos() const;
    bool eliminar(int clave);
};

class NodoBInterno : public NodoBPlusBase {
private:
    vector<NodoBPlusBase*> punteros;

public:
    NodoBInterno(int orden);
    ~NodoBInterno();
    
    int buscar_siguiente(int clave);
    void insertar_clave(int clave, NodoBPlusBase* puntero);
    NodoBInterno* dividir(int& clave_media);
    NodoBPlusBase* get_puntero(int indice);
    int num_punteros() const;
    void set_puntero(int index, NodoBPlusBase* puntero);
    void agregar_puntero(NodoBPlusBase* puntero);
};

class ArbolBPlus {
private:
    NodoBPlusBase* raiz;
    int orden;
    int accesos_disco;

    void insertar_recursivo(NodoBPlusBase* nodo, int clave, NodoGrafo* dato, 
                           int& clave_promovida, NodoBPlusBase*& nuevo_nodo);

public:
    ArbolBPlus(int orden);
    ~ArbolBPlus();
    
    void insertar(int clave, NodoGrafo* dato);
    NodoGrafo* buscar(int clave);
    bool eliminar(int clave);
    int get_accesos_disco() const;
    void resetear_accesos();
    
private:
    void destruir_arbol(NodoBPlusBase* nodo);
    bool eliminar_recursivo(NodoBPlusBase* nodo, int clave);
};

#endif