#pragma once
#include "../../components/physics_components.h"
#include "../../components/render_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include "shaders.h"
#include <cglm/cglm.h>
#include <stdint.h>
#include <sys/types.h>

uint render_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                    ComponentMask *entityComponentMasks);

void render_RenderComponent(ECS *ecs, uint8_t entityID, uint shaderProgram);

int generateRectangleVertices(float *vertices, float x, float y, float width,
                              float height);

unsigned int compileAndLinkShaders(char *vertexShaderPath,
                                   char *fragmentShaderPath);
