#ifndef NODOGRAFO_H
#define NODOGRAFO_H

#include <vector>
#include <string>

using namespace std;

class NodoGrafo {
protected:
    int id;
    vector<int> padres;

public:
    NodoGrafo(int id);
    virtual ~NodoGrafo();
    
    int get_id() const;
    virtual bool es_directorio() const = 0;
    vector<int> lista_padres() const;
    void agregar_padre(int id_padre);
    void eliminar_padre(int id_padre);
};

class NodoDirectorio : public NodoGrafo {
private:
    vector<int> hijos;

public:
    NodoDirectorio(int id);
    bool es_directorio() const override;
    vector<int> lista_hijos() const;
    void agregar_hijo(int id_hijo);
    void eliminar_hijo(int id_hijo);
};

class NodoArchivo : public NodoGrafo {
private:
    int tamanio;
    int tipo; // 0: Imagen; 1: Documento; 2: ejecutable; 3: Video; 4: Comprimido

public:
    NodoArchivo(int id, int tamanio, int tipo);
    bool es_directorio() const override;
    int get_tamanio() const;
    int get_tipo() const;
};

#endif