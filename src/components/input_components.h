#pragma once
#include "../../include/Global.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  bool isAKeyPressed[MAX_ENTITIES];
  bool isDKeyPressed[MAX_ENTITIES];
} InputComponent;

void initInputComponent(InputComponent *inputComponent);
void setEntityInput(InputComponent *inputComponent, EntityID entityID,
                    bool isAPressed, bool isDPressed);
