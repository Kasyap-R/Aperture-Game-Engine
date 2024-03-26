#include "render_components.h"
#include "string.h"

void initMeshComponent(MeshComponent *mComponent) {
  memset(mComponent->vertexCount, 0, sizeof(mComponent->vertexCount));
}

void setEntityMesh(MeshComponent *mComponent, uint8_t entityID, unsigned int VAO, unsigned int VBO, unsigned int vertexCount) {
    mComponent->VAO[entityID] = VAO;
    mComponent->VBO[entityID] = VBO;
    mComponent->vertexCount[entityID] = vertexCount;
}