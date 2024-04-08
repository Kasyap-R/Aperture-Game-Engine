#include "physics.h"
#include <math.h>
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

void physics_CheckForCollision(ECS *ecs, EntityID brickID, EntityID ballID) {
  static bool hasCollided = false;
  TransformComponent *transformComponents = ecs->transformComponent;
  f32 xBrick, yBrick, widthBrick, heightBrick, xBall, yBall, widthBall,
      heightBall, ballRadius;
  xBrick = transformComponents->x[brickID];
  yBrick = transformComponents->y[brickID];
  widthBrick = transformComponents->xScale[brickID];
  heightBrick = transformComponents->yScale[brickID];

  f32 brickHalfWidth = widthBrick / 2.0f;
  f32 brickHalfHeight = heightBrick / 2.0f;

  f32 minXBrick = xBrick - brickHalfWidth;
  f32 maxXBrick = xBrick + brickHalfWidth;
  f32 maxYBrick = yBrick + brickHalfHeight;
  f32 minYBrick = yBrick - brickHalfHeight;

  xBall = transformComponents->x[ballID];
  yBall = transformComponents->y[ballID];
  widthBall = transformComponents->xScale[ballID];
  heightBall = transformComponents->yScale[ballID];
  ballRadius = transformComponents->xScale[ballID];

  f32 nearX = findMaxFloat(minXBrick, findMinFloat(xBall, maxXBrick));
  f32 nearY = findMaxFloat(minYBrick, findMinFloat(yBall, maxYBrick));

  f32 distance = sqrtf(pow(nearX - xBall, 2) + pow(nearY - yBall, 2));
  if (distance <= widthBall / 2) {
    // There is a collision
    if (!hasCollided) {
      printf("Collision Detected\n");
      f32 ballYVelocity = ecs->velocityComponent->vY[ballID];
      ecs->velocityComponent->vY[ballID] = -ballYVelocity;
      hasCollided = true;
    }
  }
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

f32 findMaxFloat(f32 f1, f32 f2) { return (f1 > f2) ? f1 : f2; }
f32 findMinFloat(f32 f1, f32 f2) { return (f1 < f2) ? f1 : f2; }
