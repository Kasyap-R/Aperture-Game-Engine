#pragma once
#include <stdbool.h>

typedef struct {
  bool isAKeyPressed;
  bool isDKeyPressed;
} InputComponent;

void initInputComponent(InputComponent *inputComponent);
