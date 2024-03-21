#pragma once

#include "../../include/Global.h"
#include "../components/input/input.h"
#include "../components/physics/components.h"
#include <stdint.h>

typedef uint8_t ComponentMask;

typedef enum {
  COMPONENT_INPUT,
  COMPONENT_POSITION,
  COMPONENT_VELOCITY,
  COMPONENT_COUNT,
} ComponentType;

typedef struct {
  InputComponent inputComponent[MAX_ENTITIES];
  PositionComponent positionComponent[MAX_ENTITIES];
  VelocityComponent velocityComponent[MAX_ENTITIES];
  bool entityActive[MAX_ENTITIES];
} ECS;

bool hasComponent(uint8_t entityID, ComponentType type,
                  ComponentMask *entityComponentMasks);
void addComponentToEntity(uint8_t entityID, ComponentType type,
                          ComponentMask *entityComponentMasks);
void removeComponentFromEntity(uint8_t entityID, ComponentType type,
                               ComponentMask *entityComponentMasks);

void initECS(ECS *ecs);
