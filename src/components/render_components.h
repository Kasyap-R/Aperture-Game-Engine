#pragma once
#include "../../include/Global.h"
#include <stdint.h>

typedef enum {
  SHADER_COLORED,
  SHADER_TEXTURED,
} ShaderType;

typedef struct {
  u32 VAO[MAX_ENTITIES];
  u32 VBO[MAX_ENTITIES];
  u32 vertexCount[MAX_ENTITIES];
} MeshComponent;

typedef struct {
  u32 shaderProgramID[MAX_ENTITIES];
  ShaderType shaderType[MAX_ENTITIES];
} MaterialComponent;

typedef struct {
  f32 rValue[MAX_ENTITIES];
  f32 gValue[MAX_ENTITIES];
  f32 bValue[MAX_ENTITIES];
  f32 aValue[MAX_ENTITIES];
} ColorComponent;

typedef struct {
  u32 textureID[MAX_ENTITIES];
} SpriteComponent;

void initMeshComponent(MeshComponent *meshComponent);
void initMaterialComponent(MaterialComponent *materialComponent);
void initColorComponent(ColorComponent *colorComponent);
void initSpriteComponent(SpriteComponent *spriteComponent);
void setEntityMesh(MeshComponent *meshComponent, EntityID entityID, u32 VAO,
                   u32 VBO, u32 vertexCount);
void setEntityMaterial(MaterialComponent *materialComponent, EntityID entityID,
                       u32 shaderProgramID, ShaderType shaderType);
void setEntityColor(ColorComponent *colorComponent, EntityID entityID,
                    f32 rValue, f32 gValue, f32 bValue, f32 aValue);
void setEntitySprite(SpriteComponent *spriteComponent, EntityID entityID,
                     u32 textureID);
