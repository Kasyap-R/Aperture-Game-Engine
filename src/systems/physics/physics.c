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

bool physics_CheckForCollision(ECS *ecs, EntityID brickID, EntityID ballID) {
  static i32 lastCollidedBrick = -1;
  TransformComponent *transformComponents = ecs->transformComponent;
  if (brickID == ballID || brickID == lastCollidedBrick) {
    return false;
  }
  if (!checkRectangleCircleCollision(transformComponents, brickID, ballID)) {
    return false;
  }
  f32 xBrick, widthBrick, xCircle;
  xBrick = transformComponents->x[brickID];
  widthBrick = transformComponents->xScale[brickID];
  xCircle = transformComponents->x[ballID];
  f32 xMax = widthBrick / 2.0f + widthBrick * 0.1f;
  f32 xMin = -widthBrick / 2.0f - widthBrick * 0.1f;
  f32 xDiff = xCircle - xBrick;
  f32 xDiffNorm = 2.0f * ((xDiff - xMin) / (xMax - xMin)) - 1.0f;
  f32 maxXVelocity = 0.04;
  if (xCircle > xMax) {
    printf("GODDAMNIT\n");
  }
  f32 newXVelocity = xDiffNorm * maxXVelocity;
  f32 ballYVelocity = ecs->velocityComponent->vY[ballID];
  ecs->velocityComponent->vX[ballID] = newXVelocity;
  ecs->velocityComponent->vY[ballID] = -ballYVelocity;
  lastCollidedBrick = brickID;
  printf("Normalized Distance: %.5f\n", xDiffNorm);
  return true;
}

bool checkRectangleCircleCollision(TransformComponent *transformComponents,
                                   EntityID rectangleID, EntityID circleID) {
  f32 xBrick, yBrick, widthBrick, heightBrick, xCircle, yCircle, widthCircle,
      heightCircle, circleRadius, minXBrick, maxXBrick, maxYBrick, minYBrick;
  xBrick = transformComponents->x[rectangleID];
  yBrick = transformComponents->y[rectangleID];
  widthBrick = transformComponents->xScale[rectangleID];
  heightBrick = transformComponents->yScale[rectangleID];

  f32 brickHalfWidth = widthBrick / 2.0f;
  f32 brickHalfHeight = heightBrick / 2.0f;

  minXBrick = xBrick - brickHalfWidth;
  maxXBrick = xBrick + brickHalfWidth;
  maxYBrick = yBrick + brickHalfHeight;
  minYBrick = yBrick - brickHalfHeight;

  xCircle = transformComponents->x[circleID];
  yCircle = transformComponents->y[circleID];
  circleRadius = transformComponents->xScale[circleID] / 2.0f;

  f32 nearX = findMaxFloat(minXBrick, findMinFloat(xCircle, maxXBrick));
  f32 nearY = findMaxFloat(minYBrick, findMinFloat(yCircle, maxYBrick));

  f32 distance = sqrtf(pow(nearX - xCircle, 2) + pow(nearY - yCircle, 2));

  return (distance <= circleRadius);
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
