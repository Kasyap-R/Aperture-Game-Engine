#include "debug.h"

void printFloatArray(float *arr, size_t length) {
  for (int i = 0; i < length; i++) {
    printf("Index %d: %.5f\n", i, arr[i]);
  }
}
