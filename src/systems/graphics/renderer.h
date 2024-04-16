#pragma once
#include "../../components/physics_components.h"
#include "../../components/render_components.h"
#include "../../core/ECS.h"
#include "../../core/Engine.h"
#include "../../data_structures/hashmap.h"
#include "shaders.h"
#include "shapes.h"
#include <cglm/cglm.h>
#include <stdint.h>
#include <sys/types.h>

// Called Upon Initiation of the Engine
void render_LoadShaders();
void render_init_projection_matrix();

// Functions to be called externally by the user/engine
void render_RenderEntity(ECS *ecs, EntityID entityID);
void render_init_rec_prism(ECS *ecs, EntityID entityID, ShaderType shaderType,
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
void render_calculate_view_matrix(ECS *ecs, EntityID cameraID);

// Functions for use internally within the render system
void generateVertexBuffers(ShaderType shaderType, f32 *vertices,
                           u32 vertArrayLength, u32 *VAO, u32 *VBO);
void init_rec_prism_EBO(u32 *VAO, u32 *EBO);
void init_textured_VAO();
void init_colored_VAO();
void draw_elements(ECS *ecs, EntityID entityID);
