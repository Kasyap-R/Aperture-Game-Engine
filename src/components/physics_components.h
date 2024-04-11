#pragma once

#include "../../include/Global.h"
#include "string.h"
#include <stdint.h>

typedef struct {
  f32 x[MAX_ENTITIES];
  f32 y[MAX_ENTITIES];
  f32 z[MAX_ENTITIES];
  f32 xScale[MAX_ENTITIES];
  f32 yScale[MAX_ENTITIES];
  f32 zScale[MAX_ENTITIES];
} TransformComponent;

typedef struct {
  f32 vX[MAX_ENTITIES];
  f32 vY[MAX_ENTITIES];
  f32 vZ[MAX_ENTITIES];
} VelocityComponent;

void initVelocityComponent(VelocityComponent *vComponent);
void initTransformComponent(TransformComponent *tComponent);
