#include "hashmap.h"

HashMap *ds_createHashMap() { return malloc(sizeof(HashMap)); }

u32 getHash(u32 key) { return key % 100; }

u32 ds_getHashMapValue(HashMap *hashMap, u32 key) {
  u32 hashValue = getHash(key);
  return hashMap->values[hashValue];
}

void ds_insertHashMap(HashMap *hashMap, u32 key, u32 value) {
  u32 hashValue = getHash(key);
  hashMap->keys[hashValue] = key;
  hashMap->values[hashValue] = value;
}
