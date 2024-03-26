#pragma once
#include "../../components/physics_components.h"
#include "../../components/render_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"

#include <stdint.h>

void render_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                    ComponentMask *entityComponentMasks);

void render_RenderComponent(ECS *ecs, uint8_t entityID);

int generateRectangleVertices(float *vertices, float x, float y, float width,
                              float height);
