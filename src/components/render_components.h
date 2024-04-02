#pragma once
#include "../../include/Global.h"
#include <stdint.h>

typedef enum {
  SHADER_COLORED,
  SHADER_TEXTURED,
} ShaderType;

typedef struct {
  unsigned int VAO[MAX_ENTITIES];
  unsigned int VBO[MAX_ENTITIES];
  unsigned int vertexCount[MAX_ENTITIES];
} MeshComponent;

typedef struct {
  unsigned int shaderProgramID[MAX_ENTITIES];
  float rValue[MAX_ENTITIES];
  float gValue[MAX_ENTITIES];
  float bValue[MAX_ENTITIES];
  float aValue[MAX_ENTITIES];
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

void initMeshComponent(MeshComponent *mComponent);
void initMaterialComponent(MaterialComponent *matComponent);
void initColorComponent(ColorComponent *colorComponent);
void initSpriteComponent(SpriteComponent *spriteComponent);
void setEntityMesh(MeshComponent *mComponent, uint8_t entityID,
                   unsigned int VAO, unsigned int VBO,
                   unsigned int vertexCount);
void setEntityMaterial(MaterialComponent *matComponent, uint8_t entityID,
                       unsigned int shaderProgramID, float rValue, float gValue,
                       float bValue, float aValue, ShaderType shaderType);
void setEntityColor(ColorComponent *colorComponent, u8 entityID, f32 rValue,
                    f32 gValue, f32 bValue, f32 aValue);
void setEntitySprite(SpriteComponent *spriteComponent, u8 entityID,
                     u32 textureID);
