#pragma once
#include <stdbool.h>

typedef struct {
  bool isAKeyPressed;
  bool isDKeyPressed;
} InputComponent;

void initInputComponent(InputComponent *inputComponent);
void setEntityInput(InputComponent *inputComponent, bool isAPressed, bool isDPressed);
