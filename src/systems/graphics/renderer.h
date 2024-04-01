#pragma once
#include "../../components/physics_components.h"
#include "../../components/render_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include "shaders.h"
#include <cglm/cglm.h>
#include <stdint.h>
#include <sys/types.h>

void render_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                    ComponentMask *entityComponentMasks);

void render_RenderComponent(ECS *ecs, uint8_t entityID);

int generateRectangleVertices(float *vertices, float x, float y, float width,
                              float height);

void setupRectangleGeometry(TransformComponent *transformComponent,
                            uint8_t entityID, float *vertices,
                            unsigned int *VAO, unsigned int *VBO);

unsigned int compileAndLinkShaders(char *vertexShaderPath,
                                   char *fragmentShaderPath);
