#pragma once
#include "../../../include/Global.h"
#include "../../components/physics_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include <stdint.h>

void physics_InstantiatePlayerEntity(ECS *ecs, EntityID playerEntityID,
                                     ComponentMask *entityComponentMasks);

void physics_InstantiateBallEntity(ECS *ecs, EntityID ballEntityID,
                                   ComponentMask *entityComponentMasks);

void physics_UpdatePlayerEntity(ECS *ecs, EntityID entityID);
