#include "physics.h"
#include <stdint.h>
#include <stdio.h>

void physics_InstantiatePlayerEntity(ECS *ecs, EntityID playerEntityID,
                                     ComponentMask *entityComponentMasks) {
  addComponentToEntity(ecs, playerEntityID, COMPONENT_INPUT,
                       entityComponentMasks);
  addComponentToEntity(ecs, playerEntityID, COMPONENT_VELOCITY,
                       entityComponentMasks);
  addComponentToEntity(ecs, playerEntityID, COMPONENT_TRANFORM,
                       entityComponentMasks);
  f32 xPos = 0.0f;
  f32 yPos = -0.25f;
  f32 width = 0.3f;
  f32 height = 0.1f;
  f32 rotation = 0.0f;
  f32 xVelocity = 0.0f;
  f32 yVelocity = 0.0f;
  setEntityTransform(ecs->transformComponent, playerEntityID, xPos, yPos, xPos,
                     yPos, width, height, rotation);
  setEntityVelocity(ecs->velocityComponent, playerEntityID, xVelocity,
                    yVelocity);
}

void physics_UpdatePlayerEntity(ECS *ecs, EntityID entityID) {
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
    f32 xVelocity = ecs->velocityComponent->vX[entityID];
    f32 currentXPos = ecs->transformComponent->x[entityID];
    f32 currentYPos = ecs->transformComponent->y[entityID];
    ecs->transformComponent->xPrev[entityID] = currentXPos;
    ecs->transformComponent->yPrev[entityID] = currentYPos;
    currentXPos += xVelocity * 0.10f;
    ecs->transformComponent->x[entityID] = currentXPos;
  }
}
