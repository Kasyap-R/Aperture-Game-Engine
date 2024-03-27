#include "physics.h"
#include <stdint.h>

void physics_InstantiatePlayerEntity(ECS *ecs, uint8_t playerEntityID,
                                     ComponentMask *entityComponentMasks) {
  addComponentToEntity(playerEntityID, COMPONENT_INPUT, entityComponentMasks);
  addComponentToEntity(playerEntityID, COMPONENT_VELOCITY,
                       entityComponentMasks);
  addComponentToEntity(playerEntityID, COMPONENT_TRANFORM,
                       entityComponentMasks);
  float xPos = 0.5f;
  float yPos = 0.25f;
  float width = 0.3f;
  float height = 0.1f;
  float rotation = 0.0f;
  float xVelocity = 0.0f;
  float yVelocity = 0.0f;
  setEntityTransform(ecs->transformComponent, playerEntityID, xPos, yPos, width,
                     height, rotation);
  setEntityVelocity(ecs->velocityComponent, playerEntityID, xVelocity,
                    yVelocity);
}
