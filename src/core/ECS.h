#pragma once

#include "../../include/Global.h"
#include "../components/render_components.h"
#include "../components/input_components.h"
#include "../components/physics_components.h"
#include <stdint.h>

typedef uint8_t ComponentMask;

typedef enum {
  COMPONENT_INPUT,
  COMPONENT_TRANFORM,
  COMPONENT_VELOCITY,
  COMPONENT_MESH,
  COMPONENT_COUNT,
} ComponentType;

typedef struct {
  InputComponent inputComponent[MAX_ENTITIES];
  TransformComponent transformComponent[MAX_ENTITIES];
  VelocityComponent velocityComponent[MAX_ENTITIES];
  MeshComponent meshComponent[MAX_ENTITIES];
  bool entityActive[MAX_ENTITIES];
} ECS;

bool hasComponent(uint8_t entityID, ComponentType type,
                  ComponentMask *entityComponentMasks);
void addComponentToEntity(uint8_t entityID, ComponentType type,
                          ComponentMask *entityComponentMasks);
void removeComponentFromEntity(uint8_t entityID, ComponentType type,
                               ComponentMask *entityComponentMasks);

void initECS(ECS *ecs);
