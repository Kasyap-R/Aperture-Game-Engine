#pragma once
#include "../../../include/Global.h"
#include "../../components/physics_components.h"
#include "../../core/ECS.h"
#include <stdint.h>

typedef struct {
  f32 xPos;
  f32 yPos;
  f32 width;
  f32 height;
  f32 rotation;
  f32 xVelocity;
  f32 yVelocity;
} PhysicsAttributes;

void physics_InstantiateEntity(ECS *ecs, EntityID entityID,
                               PhysicsAttributes physicsAttributes,
                               ComponentMask *entityComponentMasks);

void physics_UpdateEntityPosition(ECS *ecs, EntityID entityID);

void physics_ProcessInput(ECS *ecs, EntityID entityID);

bool physics_CheckForCollision(ECS *ecs, EntityID brickID, EntityID ballID);

bool physics_check_rectangle_circle_collision(
    TransformComponent *transformComponents, EntityID rectangleID,
    EntityID circleID);

void physics_update_velocity(VelocityComponent *vComponents, EntityID entityID,
                             f32 xVelocity, f32 yVelocity, f32 zVelocity);

f32 findMaxFloat(f32 f1, f32 f2);

f32 findMinFloat(f32 f1, f32 f2);
