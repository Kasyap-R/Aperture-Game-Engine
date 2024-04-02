#pragma once
#include "../../include/Global.h"

#include <stdint.h>
#include <stdlib.h>

typedef struct {
  u32 keys[100];
  u32 values[100];
} HashMap;

HashMap *ds_createHashMap();
u32 getHash(u32 key);
void ds_insertHashMap(HashMap *hashMap, u32 key, u32 value);
