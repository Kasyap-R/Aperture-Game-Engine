#pragma once

#include "../../include/Global.h"
#include "string.h"
#include <stdint.h>

typedef struct {
  f32 xPrev[MAX_ENTITIES];
  f32 yPrev[MAX_ENTITIES];
  f32 x[MAX_ENTITIES];
  f32 y[MAX_ENTITIES];
  f32 xScale[MAX_ENTITIES];
  f32 yScale[MAX_ENTITIES];
  f32 rotation[MAX_ENTITIES];
} TransformComponent;

typedef struct {
  f32 vX[MAX_ENTITIES];
  f32 vY[MAX_ENTITIES];
} VelocityComponent;

void initVelocityComponent(VelocityComponent *vComponent);
void initTransformComponent(TransformComponent *tComponent);
void setEntityVelocity(VelocityComponent *velocityComponent, EntityID entityID,
                       f32 xVelocity, f32 yVelocity);
void setEntityTransform(TransformComponent *transformComponent,
                        EntityID entityID, f32 xPrevPosition, f32 yPrevPosition,
                        f32 xPosition, f32 yPosition, f32 xScale, f32 yScale,
                        f32 rotation);
