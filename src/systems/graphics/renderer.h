#pragma once
#include "../../components/physics_components.h"
#include "../../components/render_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include "shaders.h"
#include <cglm/cglm.h>
#include <stdint.h>
#include <sys/types.h>

void render_InstantiatePlayerEntity(ECS *ecs, EntityID entityID,
                                    ComponentMask *entityComponentMasks);

void render_RenderComponent(ECS *ecs, EntityID entityID);

int generateRectangleVertices(f32 *vertices, f32 x, f32 y, f32 width,
                              f32 height);

void setupRectangleGeometry(TransformComponent *transformComponent,
                            EntityID entityID, f32 *vertices, u32 *VAO,
                            u32 *VBO);

u32 compileAndLinkShaders(char *vertexShaderPath, char *fragmentShaderPath);
