#pragma once
#include "../../components/physics_components.h"
#include "../../components/render_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include "../../data_structures/hashmap.h"
#include "shaders.h"
#include <cglm/cglm.h>
#include <stdint.h>
#include <sys/types.h>

// Called Upon Initiation of the Engine
void render_LoadShaders();

// Functions to be called externally by the user/engine
void render_RenderEntity(ECS *ecs, EntityID entityID);
void render_InstantiateRectangleEntity(ECS *ecs, EntityID entityID,
                                       ShaderType shaderType,
                                       ComponentMask *entityComponentMasks);
void render_InstantiateCircleEntity(ECS *ecs, EntityID entityID,
                                    ShaderType shaderType,
                                    ComponentMask *entityComponentMasks);
void render_AddSpriteComponent(ECS *ecs, EntityID entityID,
                               ComponentMask *entityComponentMasks,
                               char *texturePath);
void render_AddColorComponent(ECS *ecs, EntityID entityID,
                              ComponentMask *entityComponentMasks, f32 rValue,
                              f32 gValue, f32 bValue, f32 aValue);

// Functions for use internally within the render system
i32 generateTexturedRectangleVertices(f32 **vertices, f32 width, f32 height);
i32 generateColoredRectangleVertices(f32 **vertices, f32 width, f32 height);
i32 generateTexturedCircleVertices(f32 **vertices, f32 radius);
i32 generateColoredCircleVertices(f32 **vertices, f32 radius);

u32 compileAndLinkShaders(char *vertexShaderPath, char *fragmentShaderPath);

void generateVertexBuffers(ShaderType shaderType, f32 *vertices,
                           u32 vertArrayLength, u32 *VAO, u32 *VBO,
                           u32 *numVerts);
