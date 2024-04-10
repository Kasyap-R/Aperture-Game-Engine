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

// This should implement an algorithm to check for all potential collisions in a
// scene
bool physics_CheckForCollision(ECS *ecs, EntityID brickID, EntityID ballID) {
  return true;
}

bool physics_check_rectangle_circle_collision(
    TransformComponent *transformComponents, EntityID rectangleID,
    EntityID circleID) {
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

void physics_update_velocity(VelocityComponent *vComponents, EntityID entityID,
                             f32 xVelocity, f32 yVelocity, f32 zVelocity) {
  setEntityVelocity(vComponents, entityID, xVelocity, yVelocity);
}

void physics_UpdateEntityPosition(ECS *ecs, EntityID entityID) {
  f32 xVelocity = ecs->velocityComponent->vX[entityID];
  f32 yVelocity = ecs->velocityComponent->vY[entityID];
  f32 currentXPos = ecs->transformComponent->x[entityID];
  f32 currentYPos = ecs->transformComponent->y[entityID];

  // printf("EntityID: %d\nX Velocity: %.3f\n", entityID, xVelocity);
  currentXPos += xVelocity * SEC_PER_UPDATE;
  currentYPos += yVelocity * SEC_PER_UPDATE;

  ecs->transformComponent->x[entityID] = currentXPos;
  ecs->transformComponent->y[entityID] = currentYPos;
}

f32 findMaxFloat(f32 f1, f32 f2) { return (f1 > f2) ? f1 : f2; }
f32 findMinFloat(f32 f1, f32 f2) { return (f1 < f2) ? f1 : f2; }
