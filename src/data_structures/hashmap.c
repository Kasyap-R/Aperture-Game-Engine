#include "hashmap.h"
#include <string.h>

HashMap *createHashMap() { return malloc(sizeof(HashMap)); }

void initHashMap(HashMap *hashMap) {
  memset(hashMap->keys, 0, sizeof(hashMap->keys));
  memset(hashMap->values, 0, sizeof(hashMap->values));
}

u32 getHash(u32 key) { return key % 100; }

u32 ds_getHashMapValue(HashMap **hashMap, u32 key) {
  if (*hashMap == NULL) {
    printf("HashMap is uninitialized");
    exit(1);
  }
  u32 hashValue = getHash(key);
  return (*hashMap)->values[hashValue];
}

void ds_insertHashMap(HashMap **hashMap, u32 key, u32 value) {
  if (*hashMap == NULL) {
    *hashMap = createHashMap();
    initHashMap(*hashMap);
  }
  u32 hashValue = getHash(key);
  (*hashMap)->keys[hashValue] = key;
  (*hashMap)->values[hashValue] = value;
}
