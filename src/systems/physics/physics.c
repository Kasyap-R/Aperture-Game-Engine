#include "physics.h"
#include <stdio.h>

void physics_InstantiateEntity(ECS *ecs, EntityID entityID,
                               PhysicsAttributes physicsAttributes,
                               ComponentMask *entityComponentMasks) {
  {
    f32 xPos = physicsAttributes.xPos;
    f32 yPos = physicsAttributes.yPos;
    f32 xScale = physicsAttributes.width;
    f32 yScale = physicsAttributes.height;
    f32 rotation = physicsAttributes.rotation;
    f32 xVelocity = physicsAttributes.xVelocity;
    f32 yVelocity = physicsAttributes.yVelocity;

    setEntityTransform(ecs->transformComponent, entityID, xPos, yPos, xScale,
                       yScale, rotation);
    setEntityVelocity(ecs->velocityComponent, entityID, xVelocity, yVelocity);
  }

  addComponentToEntity(ecs, entityID, COMPONENT_VELOCITY, entityComponentMasks);
  addComponentToEntity(ecs, entityID, COMPONENT_TRANFORM, entityComponentMasks);
}

void physics_ProcessInput(ECS *ecs, EntityID entityID) {

  bool isAKeyPressed = ecs->inputComponent->isAKeyPressed[entityID];
  bool isDKeyPressed = ecs->inputComponent->isDKeyPressed[entityID];
  if (isAKeyPressed && isDKeyPressed) {
    setEntityVelocity(ecs->velocityComponent, entityID, 0.0f, 0.0f);
  } else if (isAKeyPressed) {
    setEntityVelocity(ecs->velocityComponent, entityID, -0.05f, 0.0f);
  } else if (isDKeyPressed) {
    setEntityVelocity(ecs->velocityComponent, entityID, 0.05f, 0.0f);
  } else {
    setEntityVelocity(ecs->velocityComponent, entityID, 0.0f, 0.0f);
  }
}

void physics_UpdateEntityPosition(ECS *ecs, EntityID entityID) {
  f32 xVelocity = ecs->velocityComponent->vX[entityID];
  f32 yVelocity = ecs->velocityComponent->vY[entityID];
  f32 currentXPos = ecs->transformComponent->x[entityID];
  f32 currentYPos = ecs->transformComponent->y[entityID];

  // printf("EntityID: %d\nX Velocity: %.3f\n", entityID, xVelocity);
  currentXPos += xVelocity * 0.10f;
  currentYPos += yVelocity * 0.10f;

  ecs->transformComponent->x[entityID] = currentXPos;
  ecs->transformComponent->y[entityID] = currentYPos;
}
