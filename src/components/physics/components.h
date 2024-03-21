#pragma once

#include "../../../include/Global.h"
#include "string.h"

typedef struct {
  float x[MAX_ENTITIES];
  float y[MAX_ENTITIES];
} PositionComponent;

typedef struct {
  float vX[MAX_ENTITIES];
  float vY[MAX_ENTITIES];
} VelocityComponent;

void initVelocityComponent(VelocityComponent *vComponent);
void initPositionComponent(PositionComponent *pComponent);
