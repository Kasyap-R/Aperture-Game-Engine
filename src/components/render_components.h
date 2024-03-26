#pragma once
#include "../../include/Global.h"
#include <stdint.h>

typedef struct {
  unsigned int VAO[MAX_ENTITIES]; // Vertex Array object ID
  unsigned int VBO[MAX_ENTITIES];
  unsigned int vertexCount[MAX_ENTITIES]; // Num of Vertices
} MeshComponent;

void initMeshComponent(MeshComponent *mComponent);
void setEntityMesh(MeshComponent *mComponent, uint8_t entityID,
                   unsigned int VAO, unsigned int VBO,
                   unsigned int vertexCount);
