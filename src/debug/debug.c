#include "debug.h"

void printFloatArray(f32 *arr, size_t length) {
  for (u8 i = 0; i < length; i++) {
    printf("Index %d: %.5f\n", i, arr[i]);
  }
}
