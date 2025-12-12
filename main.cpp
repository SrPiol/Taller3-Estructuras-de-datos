#include "SistemaArchivos.h"
#include <iostream>
#include <limits>

using namespace std;

void mostrar_menu() {
    cout << "\n========== SISTEMA DE ARCHIVOS ==========" << endl;
    cout << "1. Crear directorio" << endl;
    cout << "2. Crear archivo" << endl;
    cout << "3. Listar contenido de directorio" << endl;
    cout << "4. Eliminar archivo" << endl;
    cout << "5. Obtener rutas completas de archivo" << endl;
    cout << "6. Calcular espacio ocupado" << endl;
    cout << "7. Buscar archivo o directorio" << endl;
    cout << "8. Mostrar estadísticas" << endl;
    cout << "9. Modo de prueba (crear estructura ejemplo)" << endl;
    cout << "0. Salir" << endl;
    cout << "=========================================" << endl;
    cout << "Seleccione una opción: ";
}

void limpiar_buffer() {//___________________________________________
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void modo_prueba(SistemaArchivos& sistema) {
    cout << "\n=== Prueba ===" << endl;
    
    // Crear raíz
    int raiz = sistema.crear_nodo(-1, true);
    cout << "Raíz creada con ID: " << raiz << endl;
    
    // Crear directorios
    int docs = sistema.crear_nodo(raiz, true);
    cout << "Directorio 'Documentos' creado con ID: " << docs << endl;
    
    int imgs = sistema.crear_nodo(raiz, true);
    cout << "Directorio 'Imágenes' creado con ID: " << imgs << endl;
    
    int videos = sistema.crear_nodo(raiz, true);
    cout << "Directorio 'Videos' creado con ID: " << videos << endl;
    
    // Crear archivos en documentos
    int doc1 = sistema.crear_nodo(docs, false, 150, 1);
    cout << "Archivo documento.txt creado con ID: " << doc1 << endl;
    
    int doc2 = sistema.crear_nodo(docs, false, 250, 1);
    cout << "Archivo reporte.pdf creado con ID: " << doc2 << endl;
    
    // Crear archivos en imgs
    int img1 = sistema.crear_nodo(imgs, false, 1024, 0);
    cout << "Archivo foto1.jpg creado con ID: " << img1 << endl;
    
    int img2 = sistema.crear_nodo(imgs, false, 2048, 0);
    cout << "Archivo foto2.png creado con ID: " << img2 << endl;
    
    // Crear archivo en videos
    int vid1 = sistema.crear_nodo(videos, false, 10240, 3);
    cout << "Archivo video1.mp4 creado con ID: " << vid1 << endl;
    
    // Crear subdirectorio en docs
    int trabajo = sistema.crear_nodo(docs, true);
    cout << "Subdirectorio 'Trabajo' creado con ID: " << trabajo << endl;
    
    int doc3 = sistema.crear_nodo(trabajo, false, 500, 1);
    cout << "Archivo proyecto.docx creado con ID: " << doc3 << endl;
    
    cout << "\n=== ESTRUCTURA CREADA EXITOSAMENTE ===" << endl;
    cout << "ID Raíz: " << raiz << endl;
    cout << "ID Documentos: " << docs << endl;
    cout << "ID Imágenes: " << imgs << endl;
    cout << "ID Videos: " << videos << endl;
}

int main() {
    int orden_bplus = 4;
    cout << "Ingrese el orden del árbol B+ (mayor a 3): ";
    cin >> orden_bplus;
    
    while (orden_bplus < 3) {
        
        limpiar_buffer();
        cout << "Orden mínimo es 3." << endl;
        cin >> orden_bplus;
    }
    
    SistemaArchivos sistema(orden_bplus);
    int opcion;
    
    do {
        mostrar_menu();
        limpiar_buffer();
        cin >> opcion;
        
        while (cin.fail() || opcion > 9 || opcion < 0) {
            limpiar_buffer();  //___________________________________________
            cout << "\n¡Error!: Ingrese un número válido. \nSeleccione una opción: ";
            cin >> opcion;
        }
        
        switch (opcion) {
            case 1: {
                int id_padre;
                cout << "Ingrese ID del directorio padre (-1 para raíz): ";
                cin >> id_padre;
                
                int nuevo_id = sistema.crear_nodo(id_padre, true);
                cout << "Directorio creado con ID: " << nuevo_id << endl;
                break;
            }
            
            case 2: {
                int id_padre, tamanio, tipo;
                cout << "Ingrese ID del directorio padre: ";
                cin >> id_padre;
                cout << "Ingrese tamaño del archivo (KB): ";
                cin >> tamanio;
                cout << "Ingrese tipo (0:Imagen, 1:Documento, 2:Ejecutable, 3:Video, 4:Comprimido): ";
                cin >> tipo;
                
                if (tipo < 0 || tipo > 4) {
                    cout << "Tipo inválido. Usando tipo 1 (Documento)." << endl;
                    tipo = 1;
                }
                
                int nuevo_id = sistema.crear_nodo(id_padre, false, tamanio, tipo);
                cout << "Archivo creado con ID: " << nuevo_id << endl;
                break;
            }
            
            case 3: {
                int id_dir;
                cout << "Ingrese ID del directorio: ";
                cin >> id_dir;
                sistema.listar_contenido(id_dir);
                break;
            }
            
            case 4: {
                int id_archivo, id_padre;
                cout << "Ingrese ID del archivo: ";
                cin >> id_archivo;
                cout << "Ingrese ID del directorio padre: ";
                cin >> id_padre;
                sistema.eliminar_archivo(id_archivo, id_padre);
                break;
            }
            
            case 5: {
                int id_archivo;
                cout << "Ingrese ID del archivo: ";
                cin >> id_archivo;
                
                vector<string> rutas = sistema.obtener_rutas_completas(id_archivo);
                
                if (rutas.empty()) {
                    cout << "No se encontraron rutas para el archivo." << endl;
                } else {
                    cout << "\nRutas completas del archivo " << id_archivo << ":" << endl;
                    for (const string& ruta : rutas) {
                        cout << "  " << ruta << endl;
                    }
                }
                break;
            }
            
            case 6: {
                int id_dir;
                cout << "Ingrese ID del directorio: ";
                cin >> id_dir;
                
                int espacio = sistema.calcular_espacio_ocupado(id_dir);
                cout << "Espacio ocupado: " << espacio << " KB" << endl;
                break;
            }
            
            case 7: {
                int id;
                cout << "Ingrese ID del nodo a buscar: ";
                cin >> id;
                
                NodoGrafo* nodo = sistema.buscar_nodo_grafo(id);
                
                if (nodo) {
                    cout << "Nodo encontrado!" << endl;
                    if (nodo->es_directorio()) {
                        cout << "Tipo: Directorio" << endl;
                    } else {
                        NodoArchivo* archivo = dynamic_cast<NodoArchivo*>(nodo);
                        string tipos[] = {"Imagen", "Documento", "Ejecutable", "Video", "Comprimido"};
                        cout << "Tipo: Archivo (" << tipos[archivo->get_tipo()] << ")" << endl;
                        cout << "Tamaño: " << archivo->get_tamanio() << " KB" << endl;
                    }
                } else {
                    cout << "Nodo no encontrado." << endl;
                }
                break;
            }
            
            case 8: {
                sistema.mostrar_estadisticas();
                break;
            }
            
            case 9: {
                modo_prueba(sistema);
                break;
            }
            
            case 0: {
                cout << "Saliendo del sistema..." << endl;
                break;
            }
            
            default: {
                cout << "No deberias ver esto :/. algo pasó en el while" << endl;
                opcion = -1;
            }
        }
        
        // limpiar_buffer(); //___________________________________________

        
    } while (opcion != 0);
    
    return 0;
}