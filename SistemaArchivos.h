#ifndef SISTEMAARCHIVOS_H
#define SISTEMAARCHIVOS_H

#include "ArbolBPlus.h"
#include "NodoGrafo.h"
#include <string>
#include <vector>
#include <queue>

using namespace std;

class SistemaArchivos {
private:
    ArbolBPlus* indice;
    int siguiente_id;

public:
    SistemaArchivos(int orden_bplus);
    ~SistemaArchivos();
    
    void insertar_nodo_grafo(int clave, NodoGrafo* nodo_grafo);
    NodoGrafo* buscar_nodo_grafo(int clave);
    int crear_nodo(int id_padre, bool es_directorio, int tamanio = 0, int tipo = 0);
    void eliminar_archivo(int id_archivo, int id_directorio_padre);
    void listar_contenido(int id_directorio);
    vector<string> obtener_rutas_completas(int id_archivo);
    int calcular_espacio_ocupado(int id_directorio);
    
    int get_siguiente_id();
    void mostrar_estadisticas();
    
private:
    void obtener_rutas_recursivo(int id_actual, string ruta_actual, 
                                 vector<string>& rutas);
};

#endif