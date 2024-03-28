#include "physics.h"
#include <stdint.h>
#include <stdio.h>

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

void physics_UpdatePlayerEntity(ECS *ecs, uint8_t entityID) {
  {
    bool isAKeyPressed = ecs->inputComponent->isAKeyPressed[entityID];
    bool isDKeyPressed = ecs->inputComponent->isDKeyPressed[entityID];
    if (isAKeyPressed && isDKeyPressed) {
      setEntityVelocity(ecs->velocityComponent, entityID, 0.0f, 0.0f);
    } else if (isAKeyPressed) {
      setEntityVelocity(ecs->velocityComponent, entityID, -0.05f, 0.0f);
    } else if (isDKeyPressed) {
      setEntityVelocity(ecs->velocityComponent, entityID, 0.05f, 0.0f);
    }
  }

  {
    float xVelocity = ecs->velocityComponent->vX[entityID];
    float currentXPos = ecs->transformComponent->x[entityID];
    currentXPos += xVelocity * 0.05f;
    fflush(stdout);
    ecs->transformComponent->x[entityID] = currentXPos;
  }
}
