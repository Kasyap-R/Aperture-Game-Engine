#include "physics.h"
#include <stdint.h>
#include <stdio.h>

void physics_InstantiatePlayerEntity(ECS *ecs, uint8_t playerEntityID,
                                     ComponentMask *entityComponentMasks) {
  addComponentToEntity(ecs, playerEntityID, COMPONENT_INPUT,
                       entityComponentMasks);
  addComponentToEntity(ecs, playerEntityID, COMPONENT_VELOCITY,
                       entityComponentMasks);
  addComponentToEntity(ecs, playerEntityID, COMPONENT_TRANFORM,
                       entityComponentMasks);
  float xPos = 0.5f;
  float yPos = 0.25f;
  float width = 0.3f;
  float height = 0.1f;
  float rotation = 0.0f;
  float xVelocity = 0.0f;
  float yVelocity = 0.0f;
  setEntityTransform(ecs->transformComponent, playerEntityID, xPos, yPos, xPos,
                     yPos, width, height, rotation);
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
      fflush(stdout);
    } else if (isDKeyPressed) {
      setEntityVelocity(ecs->velocityComponent, entityID, 0.05f, 0.0f);
    } else {
      setEntityVelocity(ecs->velocityComponent, entityID, 0.0f, 0.0f);
    }
  }

  {
    float xVelocity = ecs->velocityComponent->vX[entityID];
    float currentXPos = ecs->transformComponent->x[entityID];
    float currentYPos = ecs->transformComponent->y[entityID];
    ecs->transformComponent->xPrev[entityID] = currentXPos;
    ecs->transformComponent->yPrev[entityID] = currentYPos;
    currentXPos += xVelocity * 0.10f;
    ecs->transformComponent->x[entityID] = currentXPos;
  }
}
