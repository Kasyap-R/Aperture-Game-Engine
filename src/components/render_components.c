#include "render_components.h"
#include "string.h"

void initMeshComponent(MeshComponent *meshComponent) {
  memset(meshComponent->vertexCount, 0, sizeof(meshComponent->vertexCount));
  memset(meshComponent->VAO, 0, sizeof(meshComponent->VAO));
  memset(meshComponent->VBO, 0, sizeof(meshComponent->VBO));
  memset(meshComponent->EBO, 0, sizeof(meshComponent->EBO));
}

void initMaterialComponent(MaterialComponent *materialComponent) {
  memset(materialComponent->shaderProgramID, 0,
         sizeof(materialComponent->shaderProgramID));
  memset(materialComponent->shaderType, -1,
         sizeof(materialComponent->shaderType));
}

void initColorComponent(ColorComponent *colorComponent) {
  memset(colorComponent->rValue, 0, sizeof(colorComponent->rValue));
  memset(colorComponent->gValue, 0, sizeof(colorComponent->gValue));
  memset(colorComponent->bValue, 0, sizeof(colorComponent->bValue));
  memset(colorComponent->aValue, 0, sizeof(colorComponent->aValue));
}

void initSpriteComponent(SpriteComponent *spriteComponent) {
  memset(spriteComponent->textureID, 0, sizeof(spriteComponent->textureID));
}

void setEntityMesh(MeshComponent *meshComponent, uint8_t entityID,
                   unsigned int VAO, unsigned int VBO, u32 EBO,
                   unsigned int vertexCount) {
  meshComponent->VAO[entityID] = VAO;
  meshComponent->VBO[entityID] = VBO;
  meshComponent->EBO[entityID] = EBO;
  meshComponent->vertexCount[entityID] = vertexCount;
}

void setEntityMaterial(MaterialComponent *materialComponent, uint8_t entityID,
                       unsigned int shaderProgramID, ShaderType shaderType) {
  materialComponent->shaderProgramID[entityID] = shaderProgramID;
  materialComponent->shaderType[entityID] = shaderType;
}

void setEntityColor(ColorComponent *colorComponent, EntityID entityID,
                    f32 rValue, f32 gValue, f32 bValue, f32 aValue) {
  colorComponent->rValue[entityID] = rValue;
  colorComponent->gValue[entityID] = gValue;
  colorComponent->bValue[entityID] = bValue;
  colorComponent->aValue[entityID] = aValue;
}

void setEntitySprite(SpriteComponent *spriteComponent, EntityID entityID,
                     u32 textureID) {
  spriteComponent->textureID[entityID] = textureID;
}
