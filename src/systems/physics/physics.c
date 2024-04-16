#include "physics.h"
#include <math.h>
#include <stdio.h>

void physics_InstantiateEntity(ECS *ecs, EntityID entityID,
                               PhysicsAttributes physicsAttributes,
                               ComponentMask *entityComponentMasks) {
  {
    f32 xPos = physicsAttributes.xPos;
    f32 yPos = physicsAttributes.yPos;
    f32 zPos = physicsAttributes.zPos;
    f32 xScale = physicsAttributes.xScale;
    f32 yScale = physicsAttributes.yScale;
    f32 zScale = physicsAttributes.zScale;
    f32 xVelocity = physicsAttributes.xVelocity;
    f32 yVelocity = physicsAttributes.yVelocity;
    f32 zVelocity = physicsAttributes.zVelocity;

    physics_update_transform(ecs->transformComponent, entityID, xPos, yPos,
                             zPos, xScale, yScale, zScale);
    physics_update_velocity(ecs->velocityComponent, entityID, xVelocity,
                            yVelocity, zVelocity);
  }

  addComponentToEntity(ecs, entityID, COMPONENT_VELOCITY, entityComponentMasks);
  addComponentToEntity(ecs, entityID, COMPONENT_TRANFORM, entityComponentMasks);
}

// This should implement an algorithm to check for all potential collisions in a
// scene
bool physics_CheckForCollision(ECS *ecs, EntityID brickID, EntityID ballID) {
  return true;
}

// 2D only - deprecated
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

void physics_UpdateEntityPosition(ECS *ecs, EntityID entityID) {
  f32 xVelocity = ecs->velocityComponent->vX[entityID];
  f32 yVelocity = ecs->velocityComponent->vY[entityID];
  f32 zVelocity = ecs->velocityComponent->vZ[entityID];
  f32 currentXPos = ecs->transformComponent->x[entityID];
  f32 currentYPos = ecs->transformComponent->y[entityID];
  f32 currentZPos = ecs->transformComponent->z[entityID];

  currentXPos += xVelocity * SEC_PER_UPDATE;
  currentYPos += yVelocity * SEC_PER_UPDATE;
  currentZPos += zVelocity * SEC_PER_UPDATE;

  ecs->transformComponent->x[entityID] = currentXPos;
  ecs->transformComponent->y[entityID] = currentYPos;
  ecs->transformComponent->z[entityID] = currentZPos;
}

f32 findMaxFloat(f32 f1, f32 f2) { return (f1 > f2) ? f1 : f2; }
f32 findMinFloat(f32 f1, f32 f2) { return (f1 < f2) ? f1 : f2; }

// Both of these are setters used when the entirety of a component needs to be
// changed (usually upon instantiation)
void physics_update_velocity(VelocityComponent *vComponents, EntityID entityID,
                             f32 xVelocity, f32 yVelocity, f32 zVelocity) {
  vComponents->vX[entityID] = xVelocity;
  vComponents->vY[entityID] = yVelocity;
  vComponents->vZ[entityID] = zVelocity;
}

void physics_update_transform(TransformComponent *tComponents,
                              EntityID entityID, f32 xPos, f32 yPos, f32 zPos,
                              f32 xScale, f32 yScale, f32 zScale) {
  tComponents->x[entityID] = xPos;
  tComponents->y[entityID] = yPos;
  tComponents->z[entityID] = zPos;
  tComponents->xScale[entityID] = xScale;
  tComponents->yScale[entityID] = yScale;
  tComponents->zScale[entityID] = zScale;
}
