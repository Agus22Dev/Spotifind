#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    if (new == NULL){
        printf("Error al asignar memoria");
        exit(1);
    }
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long pos = hash(key, map->capacity);
    long posOrig = pos;
    while (map->buckets[pos] != NULL){
        if (strcasecmp(map->buckets[pos]->key, key) == 0 && map->buckets[pos]->key != NULL){
            return;
        }
        pos = (pos + 1) % map->capacity;

        if (pos == posOrig) return;
    }
    
    Pair* pairNew = createPair(key, value);

    map->buckets[pos] = pairNew;
    map->size++;
    map->current = pos;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)
    Pair **bucketOld = map->buckets;

    map->capacity *= 2;

    map->buckets =  (Pair**)malloc(sizeof(Pair*) * map->capacity);
    if (map->buckets == NULL){
        map->buckets = bucketOld;
        return;
    }

    map->size = 0;

    for (long i = 0; i < map->capacity; i++){
        map->buckets[i] = NULL;
    }

    for (long i = 0; i < map->capacity / 2; i++){
        if (bucketOld[i] != NULL){
            insertMap(map, bucketOld[i]->key, bucketOld[i]->value);
        }
    }

    free(bucketOld);

}


HashMap * createMap(long capacity) {
    HashMap * map = (HashMap*)malloc(sizeof(HashMap));
    if (map == NULL) return NULL;
    
    map->buckets = (Pair**)malloc(sizeof(Pair*)* capacity);
    if (map->buckets == NULL){
        free(map);
        return NULL;
    }
    for (long i = 0; i < capacity; i++){
        map->buckets[i] = NULL;
    }
    map->capacity = capacity;
    map->size = 0;
    map->current = -1;

    return map;
}

void eraseMap(HashMap * map,  char * key) {    
    Pair * pair = searchMap(map, key);
    if (pair != NULL){
        pair->key = NULL;
        map->size--;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long pos = hash(key, map->capacity);
    long posOri = pos;

    while (map->buckets[pos] != NULL){
        if (map->buckets[pos]->key != NULL && strcasecmp(map->buckets[pos]->key, key) == 0) {
            map->current = pos;
            return map->buckets[pos];
        }
        pos = (pos + 1) % map->capacity;

        if (pos == posOri) break;
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {
    if (map == NULL || map->buckets == NULL || map->size == 0) return NULL;
    for (long i = 0; i < map->capacity; i++){
        if (map->buckets[i] != NULL && map->buckets[i]->key != NULL){
            map->current = i;
            return map->buckets[i];
        }
    }
    map->current = -1;
    return NULL;
}

Pair * nextMap(HashMap * map) {
    long pos = map->current + 1;

    while (pos < map->capacity) {
        if (map->buckets[pos] != NULL && map->buckets[pos]->key != NULL) {
            map->current = pos;
            return map->buckets[pos];
        }
        pos++;
    }
    return NULL;
}

