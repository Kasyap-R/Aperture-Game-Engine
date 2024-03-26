#include "physics_components.h"

void initVelocityComponent(VelocityComponent *vComponent) {
  memset(vComponent->vX, 0, sizeof(vComponent->vX));
  memset(vComponent->vY, 0, sizeof(vComponent->vY));
}

void initTransformComponent(TransformComponent *tComponent) {
  memset(tComponent->x, 0, sizeof(tComponent->x));
  memset(tComponent->y, 0, sizeof(tComponent->y));
  memset(tComponent->xScale, 0, sizeof(tComponent->xScale));
  memset(tComponent->yScale, 0, sizeof(tComponent->yScale));
  memset(tComponent->rotation, 0, sizeof(tComponent->rotation));
}

void setEntityVelocity(VelocityComponent *velocityComponent, uint8_t entityID,
                       float xVelocity, float yVelocity) {
  velocityComponent->vX[entityID] = xVelocity;
  velocityComponent->vY[entityID] = yVelocity;
}

void setEntityTransform(TransformComponent *transformComponent,
                        uint8_t entityID, float xPosition, float yPosition, float xScale, float yScale, float rotation) {
  transformComponent->x[entityID] = xPosition;
  transformComponent->y[entityID] = yPosition;
  transformComponent->xScale[entityID] = xScale;
  transformComponent->yScale[entityID] = yScale;
  transformComponent->rotation[entityID] = rotation;
}