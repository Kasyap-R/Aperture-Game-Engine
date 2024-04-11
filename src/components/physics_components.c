#include "physics_components.h"

void initVelocityComponent(VelocityComponent *vComponent) {
  memset(vComponent->vX, 0, sizeof(vComponent->vX));
  memset(vComponent->vY, 0, sizeof(vComponent->vY));
}

void initTransformComponent(TransformComponent *tComponent) {
  memset(tComponent->x, 0, sizeof(tComponent->x));
  memset(tComponent->y, 0, sizeof(tComponent->y));
  memset(tComponent->z, 0, sizeof(tComponent->z));
  memset(tComponent->xScale, 0, sizeof(tComponent->xScale));
  memset(tComponent->yScale, 0, sizeof(tComponent->yScale));
  memset(tComponent->zScale, 0, sizeof(tComponent->zScale));
}
