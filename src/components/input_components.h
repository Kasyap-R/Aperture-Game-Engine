#pragma once
#include "../../include/Global.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool isWKeyPressed[MAX_ENTITIES];
  bool isSKeyPressed[MAX_ENTITIES];
  bool isAKeyPressed[MAX_ENTITIES];
  bool isDKeyPressed[MAX_ENTITIES];
} InputComponent;

void initInputComponent(InputComponent *inputComponent);
void setEntityInput(InputComponent *inputComponent, EntityID entityID,
                    bool isWPressed, bool isSPressed, bool isAPressed,
                    bool isDPressed);
