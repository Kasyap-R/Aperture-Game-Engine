#pragma once
#include "../../components/physics_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include <stdint.h>

void physics_InstantiatePlayerEntity(ECS *ecs, uint8_t playerEntityID,
                                     ComponentMask *entityComponentMasks);

void instantiateBallEntity(ECS *ecs, uint8_t ballEntityID,
                           ComponentMask *entityComponentMasks);
