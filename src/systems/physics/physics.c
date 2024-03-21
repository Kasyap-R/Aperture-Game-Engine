#include "physics.h"

void instantiatePlayerEntity(ECS *ecs, uint8_t playerEntityID,
                             ComponentMask *entityComponentMasks) {
  addComponentToEntity(playerEntityID, COMPONENT_INPUT, entityComponentMasks);
  addComponentToEntity(playerEntityID, COMPONENT_VELOCITY,
                       entityComponentMasks);
  addComponentToEntity(playerEntityID, COMPONENT_POSITION,
                       entityComponentMasks);
  setEntityPosition(ecs->positionComponent, playerEntityID, 0.5f, 0.25f);
}

void setEntityVelocity(VelocityComponent *velocityComponent, uint8_t entityID,
                       float xVelocity, float yVelocity) {
  velocityComponent->vX[entityID] = xVelocity;
  velocityComponent->vY[entityID] = yVelocity;
}

void setEntityPosition(PositionComponent *positionComponent, uint8_t entityID,
                       float xPosition, float yPosition) {
  positionComponent->x[entityID] = xPosition;
  positionComponent->y[entityID] = yPosition;
}
