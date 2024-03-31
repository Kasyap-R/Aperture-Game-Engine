#pragma once
#include "../../include/Global.h"
#include <stdint.h>

typedef struct {
  unsigned int VAO[MAX_ENTITIES]; // Vertex Array object ID
  unsigned int VBO[MAX_ENTITIES];
  unsigned int vertexCount[MAX_ENTITIES]; // Num of Vertices
} MeshComponent;

typedef struct {
  unsigned int shaderProgramID[MAX_ENTITIES];
  float rValue[MAX_ENTITIES];
  float gValue[MAX_ENTITIES];
  float bValue[MAX_ENTITIES];
  float aValue[MAX_ENTITIES];
} MaterialComponent;

void initMeshComponent(MeshComponent *mComponent);
void initMaterialComponent(MaterialComponent *matComponent);
void setEntityMesh(MeshComponent *mComponent, uint8_t entityID,
                   unsigned int VAO, unsigned int VBO,
                   unsigned int vertexCount);
void setEntityMaterial(MaterialComponent *matComponent, uint8_t entityID,
                       unsigned int shaderProgramID, float rValue, float gValue,
                       float bValue, float aValue);
