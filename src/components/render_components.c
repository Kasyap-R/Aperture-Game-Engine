#include "render_components.h"
#include "string.h"

void initMeshComponent(MeshComponent *mComponent) {
  memset(mComponent->vertexCount, 0, sizeof(mComponent->vertexCount));
}

void initMaterialComponent(MaterialComponent *matComponent) {
  memset(matComponent->shaderProgramID, 0,
         sizeof(matComponent->shaderProgramID));
  memset(matComponent->rValue, 0, sizeof(matComponent->rValue));
  memset(matComponent->gValue, 0, sizeof(matComponent->gValue));
  memset(matComponent->bValue, 0, sizeof(matComponent->bValue));
  memset(matComponent->aValue, 0, sizeof(matComponent->aValue));
}

void setEntityMesh(MeshComponent *mComponent, uint8_t entityID,
                   unsigned int VAO, unsigned int VBO,
                   unsigned int vertexCount) {
  mComponent->VAO[entityID] = VAO;
  mComponent->VBO[entityID] = VBO;
  mComponent->vertexCount[entityID] = vertexCount;
}

void setEntityMaterial(MaterialComponent *matComponent, uint8_t entityID,
                       unsigned int shaderProgramID, float rValue, float gValue,
                       float bValue, float aValue) {
  matComponent->shaderProgramID[entityID] = shaderProgramID;
  matComponent->rValue[entityID] = rValue;
  matComponent->gValue[entityID] = gValue;
  matComponent->bValue[entityID] = bValue;
  matComponent->aValue[entityID] = aValue;
}
