#pragma once
#include "../../components/physics/components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include <stdint.h>

void instantiatePlayerEntity(ECS *ecs, uint8_t playerEntityID,
                             ComponentMask *entityComponentMasks);
void instantiateBallEntity(ECS *ecs, uint8_t ballEntityID,
                           ComponentMask *entityComponentMasks);

void setEntityVelocity(VelocityComponent *velocityComponent, uint8_t entityID,
                       float xVelocity, float yVelocity);
void setEntityPosition(PositionComponent *positionComponent, uint8_t entityID,
                       float xPosition, float yPosition);
