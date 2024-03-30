#pragma once

#include "../../include/Global.h"
#include "string.h"
#include <stdint.h>

typedef struct {
  float xPrev[MAX_ENTITIES];
  float yPrev[MAX_ENTITIES];
  float x[MAX_ENTITIES];
  float y[MAX_ENTITIES];
  float xScale[MAX_ENTITIES];
  float yScale[MAX_ENTITIES];
  float rotation[MAX_ENTITIES];
} TransformComponent;

typedef struct {
  float vX[MAX_ENTITIES];
  float vY[MAX_ENTITIES];
} VelocityComponent;

void initVelocityComponent(VelocityComponent *vComponent);
void initTransformComponent(TransformComponent *tComponent);
void setEntityVelocity(VelocityComponent *velocityComponent, uint8_t entityID,
                       float xVelocity, float yVelocity);
void setEntityTransform(TransformComponent *transformComponent,
                        uint8_t entityID, float xPrevPosition,
                        float yPrevPosition, float xPosition, float yPosition,
                        float xScale, float yScale, float rotation);
