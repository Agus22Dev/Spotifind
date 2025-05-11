#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <windows.h>
#include "tdas/hashmap.h"
#include <stdbool.h>
#include "tdas/extra.h"
#include "tdas/list.h"

typedef struct {
    char id[100];
    char track[100];
    char album_name[100];
    char track_name[100];
    char artist[100];
    float tempo;
    char genre[100];
} Cancion;

HashMap* mapaGenero;
HashMap* mapaArtista;
HashMap* mapaTempo;

int compararPorNombreCancion(const void *a, const void *b) {
    Cancion *c1 = *(Cancion **)a;
    Cancion *c2 = *(Cancion **)b;
    return strcmp(c1->track_name, c2->track_name);
}


void cargarCancionesDesdeCSV() {
    FILE *archivoCSV = fopen("song_dataset_.csv", "r");
    if (archivoCSV == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char **camposLinea;
    camposLinea = leer_linea_csv(archivoCSV, ',');

    while ((camposLinea = leer_linea_csv(archivoCSV, ',')) != NULL) {
        Cancion* cancionActual = malloc(sizeof(Cancion));
        if (cancionActual == NULL) {
            perror("No se pudo asignar memoria para Cancion");
            exit(EXIT_FAILURE);
        }

        strcpy(cancionActual->id, camposLinea[0]);
        strcpy(cancionActual->track_name, camposLinea[4]);
        strcpy(cancionActual->artist, camposLinea[2]);
        strcpy(cancionActual->album_name, camposLinea[3]);
        strcpy(cancionActual->genre, camposLinea[20]);
        cancionActual->tempo = atof(camposLinea[18]);

        HashMap* submapaGenero;
        Pair* parEncontrado = searchMap(mapaGenero, cancionActual->genre);

        if (parEncontrado == NULL) {
            submapaGenero = createMap(10000);
            insertMap(mapaGenero, cancionActual->genre, submapaGenero);
        } else {
            submapaGenero = (HashMap*)parEncontrado->value;
        }
        insertMap(submapaGenero, cancionActual->id, cancionActual);

        printf("ID: %d\n", atoi(camposLinea[0]));
        printf("Titulo cancion: %s\n", camposLinea[4]);

        List* listaArtistas = split_string(camposLinea[2], ";");
        HashMap* submapaArtista;
        printf("Artistas: \n");
        if (listaArtistas != NULL) {
            for (char *artista = list_first(listaArtistas); artista != NULL; artista = list_next(listaArtistas)) {
                Pair* parEncontrado = searchMap(mapaArtista, cancionActual->artist);
                if (parEncontrado == NULL) {
                    submapaArtista = createMap(10000);
                    insertMap(mapaArtista, cancionActual->artist, submapaArtista);
                } else {
                    submapaArtista = (HashMap*)parEncontrado->value;
                }
                insertMap(submapaArtista, cancionActual->id, cancionActual);
                printf(" %s\n", artista);
            }
        }

        printf("Album: %s\n", camposLinea[3]);
        printf("Genero: %s\n", camposLinea[20]);
        printf("Tempo: %.2f\n", atof(camposLinea[18]));
        printf(" -------------------------------\n");

        char *categoriaTempo;
        if (cancionActual->tempo < 80.0) {
            categoriaTempo = "lenta";
        } else if (cancionActual->tempo <= 120.0) {
            categoriaTempo = "moderada";
        } else {
            categoriaTempo = "rapida";
        }
        Pair* parEncontradoTempo = searchMap(mapaTempo, categoriaTempo);
        List* listaPorTempo;

        if (parEncontradoTempo != NULL) {
            listaPorTempo = (List*)parEncontradoTempo->value;
        } else {
            listaPorTempo = list_create();
            insertMap(mapaTempo, strdup(categoriaTempo), listaPorTempo);
        }

        if (cancionActual != NULL && cancionActual->tempo >= 0.0) {
            list_pushBack(listaPorTempo, cancionActual);
        }
    }

    fclose(archivoCSV);
}

void buscarCancionesPorGenero() {
    char generoBuscado[100];
    Pair* parEncontrado;

    do {
        printf("Ingrese el genero deseado\n");
        printf("O presione 1 para volver al menu:\n");
        scanf(" %[^\n]", generoBuscado);
        if (strcmp(generoBuscado, "1") == 0) return;

        parEncontrado = searchMap(mapaGenero, generoBuscado);
        if (parEncontrado == NULL) printf("Genero no encontrado en el archivo\n");
    } while (parEncontrado == NULL);

    HashMap* cancionesPorGenero = (HashMap*)parEncontrado->value;

    printf("\nGénero: %s\n", generoBuscado);
    printf("----------------------\n");

    Pair* parCancion = firstMap(cancionesPorGenero);
    while (parCancion != NULL) {
        Cancion* cancionActual = (Cancion*)parCancion->value;
        printf("Artista: %s - Cancion: %s Album:(%s) Tempo:[%.1f BPM]\n", cancionActual->artist, cancionActual->track_name, cancionActual->album_name, cancionActual->tempo);
        parCancion = nextMap(cancionesPorGenero);
    }
}

void buscarCancionesPorArtista() {
    char artistaBuscado[100];
    Pair* parEncontrado;

    do {
        printf("Ingrese el Artista deseado\n");
        printf("O presione 1 para volver al menu:\n");
        scanf(" %[^\n]", artistaBuscado);
        if (strcmp(artistaBuscado, "1") == 0) return;

        parEncontrado = searchMap(mapaArtista, artistaBuscado);
        if (parEncontrado == NULL) printf("Artista no encontrado en el archivo\n");
    } while (parEncontrado == NULL);

    HashMap* cancionesPorArtista = (HashMap*)parEncontrado->value;
    List* listaOrdenada = list_create();

    Pair* parCancion = firstMap(cancionesPorArtista);
    while (parCancion != NULL) {
        Cancion* cancionActual = (Cancion*)parCancion->value;
        list_pushBack(listaOrdenada, cancionActual);
        parCancion = nextMap(cancionesPorArtista);
    }

    int n = 0;
    Cancion **arregloCanciones = malloc(sizeof(Cancion*) * 100000);
    for (Cancion* c = list_first(listaOrdenada); c != NULL; c = list_next(listaOrdenada)) {
        arregloCanciones[n++] = c;
    }

    qsort(arregloCanciones, n, sizeof(Cancion*), compararPorNombreCancion);

    printf("\nArtista: %s\n", artistaBuscado);
    printf("----------------------\n");
    for (int i = 0; i < n; i++) {
        Cancion* cancionActual = arregloCanciones[i];
        printf("Genero: %s - Cancion: %s Album: (%s) Tempo: [%.1f BPM]\n",
               cancionActual->genre,
               cancionActual->track_name,
               cancionActual->album_name,
               cancionActual->tempo);
    }

    free(arregloCanciones);
}


void buscarCancionesPorTempo() {
    char tempoBuscado[20];
    Pair* parEncontrado = NULL;

    do {
        printf("Ingrese el Tempo deseado\n");
        printf("Utiliza una de las siguientes categorias:\n");
        printf(" - lenta (<= 80 BPM)\n");
        printf(" - moderada (<= 120 BPM)\n");
        printf(" - rapida (> 120 BPM)\n");
        printf("O presione 1 para volver al menú:\n");

        scanf(" %[^\n]", tempoBuscado);
        if (strcmp(tempoBuscado, "1") == 0) return;

        if (strcmp(tempoBuscado, "lenta") == 0 || strcmp(tempoBuscado, "moderada") == 0 || strcmp(tempoBuscado, "rapida") == 0) {
            parEncontrado = searchMap(mapaTempo, tempoBuscado);
            if (parEncontrado == NULL) {
                printf("Tempo no encontrado en el archivo.\n");
            }
        } else {
            printf("Tempo ingresado no válido.\n");
            parEncontrado = NULL;
        }

    } while (parEncontrado == NULL);

    List* cancionesPorTempo = (List*)parEncontrado->value;
    List* listaOrdenada = list_create();

    for (Cancion* c = list_first(cancionesPorTempo); c != NULL; c = list_next(cancionesPorTempo)) {
        list_pushBack(listaOrdenada, c);
    }

    int n = 0;
    Cancion **arregloCanciones = malloc(sizeof(Cancion*) * 100000);
    for (Cancion* c = list_first(listaOrdenada); c != NULL; c = list_next(listaOrdenada)) {
        arregloCanciones[n++] = c;
    }

    qsort(arregloCanciones, n, sizeof(Cancion*), compararPorNombreCancion);

    printf("\nTempo: %s\n", tempoBuscado);
    printf("----------------------\n");
    for (int i = 0; i < n; i++) {
        Cancion* cancionActual = arregloCanciones[i];
        printf("Genero: %s - Cancion: %s Album: (%s) Tempo: [%.1f BPM]\n",
               cancionActual->genre,
               cancionActual->track_name,
               cancionActual->album_name,
               cancionActual->tempo);
    }

    free(arregloCanciones);
}


void mostrarMenuPrincipal() {
    puts("========================================");
    puts("     Base de Datos de Canciones         ");
    puts("========================================");
    puts("Presiona 1 para cargar archivo.csv");
    puts("Presiona 2 para Buscar por genero");
    puts("Presiona 3 para Buscar por artistas");
    puts("Presiona 4 para Buscar por tempo");
    puts("Presiona 5 para finalizar el programa");
}

int main() {
    int opcionUsuario;
    mapaGenero = createMap(600000);
    mapaArtista = createMap(600000);
    mapaTempo = createMap(10);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    do {
        mostrarMenuPrincipal();
        scanf("%d", &opcionUsuario);
        getchar();

        switch (opcionUsuario) {
            case 1: cargarCancionesDesdeCSV(); break;
            case 2: buscarCancionesPorGenero(); break;
            case 3: buscarCancionesPorArtista(); break;
            case 4: buscarCancionesPorTempo(); break;
            case 5: printf("Cerrando aplicacion....\n"); break;
            default: printf("OPCION INVALIDA. POR FAVOR REINGRESE OPCION\n"); break;
        }
    } while (opcionUsuario != 5);

    return 0;
}
