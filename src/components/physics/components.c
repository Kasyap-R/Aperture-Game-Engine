#include "components.h"

void initVelocityComponent(VelocityComponent *vComponent) {
  memset(vComponent->vX, 0, sizeof(vComponent->vX));
  memset(vComponent->vY, 0, sizeof(vComponent->vY));
}

void initPositionComponent(PositionComponent *pComponent) {
  memset(pComponent->x, 0, sizeof(pComponent->x));
  memset(pComponent->x, 0, sizeof(pComponent->x));
}
