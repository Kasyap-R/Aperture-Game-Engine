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
