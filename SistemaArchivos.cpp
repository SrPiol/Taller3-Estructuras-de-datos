#include "SistemaArchivos.h"
#include <iostream>
#include <set>

using namespace std;

SistemaArchivos::SistemaArchivos(int orden_bplus) {
    indice = new ArbolBPlus(orden_bplus);
    siguiente_id = 0;
}

SistemaArchivos::~SistemaArchivos() {
    delete indice;
}

void SistemaArchivos::insertar_nodo_grafo(int clave, NodoGrafo* nodo_grafo) {
    indice->insertar(clave, nodo_grafo);
}

NodoGrafo* SistemaArchivos::buscar_nodo_grafo(int clave) {
    return indice->buscar(clave);
}

int SistemaArchivos::crear_nodo(int id_padre, bool es_directorio, int tamanio, int tipo) {
    int nuevo_id = siguiente_id++;
    
    NodoGrafo* nuevo_nodo;
    if (es_directorio) {
        nuevo_nodo = new NodoDirectorio(nuevo_id);
    } else {
        nuevo_nodo = new NodoArchivo(nuevo_id, tamanio, tipo);
    }
    
    // Insertar en el B+
    insertar_nodo_grafo(nuevo_id, nuevo_nodo);
    
    // Si tiene padre, actualizar relaciones
    if (id_padre >= 0) {
        NodoGrafo* padre = buscar_nodo_grafo(id_padre);
        if (padre && padre->es_directorio()) {
            NodoDirectorio* dir_padre = dynamic_cast<NodoDirectorio*>(padre);
            dir_padre->agregar_hijo(nuevo_id);
            nuevo_nodo->agregar_padre(id_padre);
        }
    }
    
    return nuevo_id;
}

void SistemaArchivos::eliminar_archivo(int id_archivo, int id_directorio_padre) {
    NodoGrafo* archivo = buscar_nodo_grafo(id_archivo);
    if (!archivo) {
        cout << "Error: Archivo no encontrado." << endl;
        return;
    }
    
    // Eliminar referencia del padre
    archivo->eliminar_padre(id_directorio_padre);
    
    // Actualizar lista de hijos del padre
    NodoGrafo* padre = buscar_nodo_grafo(id_directorio_padre);
    if (padre && padre->es_directorio()) {
        NodoDirectorio* dir_padre = dynamic_cast<NodoDirectorio*>(padre);
        dir_padre->eliminar_hijo(id_archivo);
    }
    
    // Si no tiene más padres, eliminar del B+
    if (archivo->lista_padres().empty()) {
        indice->eliminar(id_archivo);
        delete archivo;
        cout << "Archivo " << id_archivo << " eliminado completamente." << endl;
    } else {
        cout << "Archivo " << id_archivo << " aún tiene referencias." << endl;
    }
}

void SistemaArchivos::listar_contenido(int id_directorio) {
    NodoGrafo* nodo = buscar_nodo_grafo(id_directorio);
    
    if (!nodo) {
        cout << "Error: Directorio no encontrado." << endl;
        return;
    }
    
    if (!nodo->es_directorio()) {
        cout << "Error: El ID no corresponde a un directorio." << endl;
        return;
    }
    
    NodoDirectorio* directorio = dynamic_cast<NodoDirectorio*>(nodo);
    vector<int> hijos = directorio->lista_hijos();
    
    cout << "\nContenido del directorio " << id_directorio << ":" << endl;
    cout << "------------------------------------------------" << endl;
    
    if (hijos.empty()) {
        cout << "(vacío)" << endl;
        return;
    }
    
    for (int id_hijo : hijos) {
        NodoGrafo* hijo = buscar_nodo_grafo(id_hijo);
        if (hijo) {
            if (hijo->es_directorio()) {
                cout << "[DIR]  ID: " << id_hijo << endl;
            } else {
                NodoArchivo* archivo = dynamic_cast<NodoArchivo*>(hijo);
                string tipos[] = {"Imagen", "Documento", "Ejecutable", "Video", "Comprimido"};
                cout << "[FILE] ID: " << id_hijo 
                         << " | Tamaño: " << archivo->get_tamanio() << " KB"
                         << " | Tipo: " << tipos[archivo->get_tipo()] << endl;
            }
        }
    }
    cout << "------------------------------------------------" << endl;
}

vector<string> SistemaArchivos::obtener_rutas_completas(int id_archivo) {
    vector<string> rutas;
    NodoGrafo* nodo = buscar_nodo_grafo(id_archivo);
    
    if (!nodo) {
        return rutas;
    }
    
    obtener_rutas_recursivo(id_archivo, "", rutas);
    return rutas;
}

void SistemaArchivos::obtener_rutas_recursivo(int id_actual, string ruta_actual,vector<string>& rutas) {
    
    NodoGrafo* nodo = buscar_nodo_grafo(id_actual);
    if (!nodo) return;
    
    string nueva_ruta = "/" + to_string(id_actual) + ruta_actual;
    
    vector<int> padres = nodo->lista_padres();
    
    if (padres.empty()) {
        rutas.push_back(nueva_ruta);
    } else {
        for (int id_padre : padres) {
            obtener_rutas_recursivo(id_padre, nueva_ruta, rutas);
        }
    }
}

int SistemaArchivos::calcular_espacio_ocupado(int id_directorio) {
    NodoGrafo* nodo = buscar_nodo_grafo(id_directorio);
    
    if (!nodo) {
        cout << "Error: Directorio no encontrado." << endl;
        return 0;
    }
    
    if (!nodo->es_directorio()) {
        cout << "Error: El ID no corresponde a un directorio." << endl;
        return 0;
    }
    
    int espacio_total = 0;
    queue<int> cola;
    set<int> visitados;
    
    cola.push(id_directorio);
    visitados.insert(id_directorio);
    
    while (!cola.empty()) {
        int id_actual = cola.front();
        cola.pop();
        
        NodoGrafo* actual = buscar_nodo_grafo(id_actual);
        if (!actual) continue;
        
        if (actual->es_directorio()) {
            NodoDirectorio* dir = dynamic_cast<NodoDirectorio*>(actual);
            vector<int> hijos = dir->lista_hijos();
            
            for (int id_hijo : hijos) {
                if (visitados.find(id_hijo) == visitados.end()) {
                    cola.push(id_hijo);
                    visitados.insert(id_hijo);
                }
            }
        } else {
            NodoArchivo* archivo = dynamic_cast<NodoArchivo*>(actual);
            espacio_total += archivo->get_tamanio();
        }
    }
    
    return espacio_total;
}

int SistemaArchivos::get_siguiente_id() {
    return siguiente_id;
}

void SistemaArchivos::mostrar_estadisticas() {
    cout << "\n=== Estadísticas del Sistema ===" << endl;
    cout << "Accesos a disco en última búsqueda: " 
              << indice->get_accesos_disco() << endl;
    cout << "Total de nodos creados: " << siguiente_id << endl;
    cout << "================================\n" << endl;
}