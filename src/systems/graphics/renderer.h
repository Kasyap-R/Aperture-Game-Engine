#pragma once
#include "../../components/physics_components.h"
#include "../../components/render_components.h"
#include "../../core/ECS.h"
#include <stdint.h>

void renderPlayer(ECS *ecs, uint8_t entityID);

int generateRectangleVertices(float *vertices, float x, float y, float width,
                              float height);