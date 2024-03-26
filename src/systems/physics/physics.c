#include "physics.h"
#include <stdint.h>

void instantiatePlayerEntity(ECS *ecs, uint8_t playerEntityID,
                             ComponentMask *entityComponentMasks) {
  addComponentToEntity(playerEntityID, COMPONENT_INPUT, entityComponentMasks);
  addComponentToEntity(playerEntityID, COMPONENT_VELOCITY,
                       entityComponentMasks);
  addComponentToEntity(playerEntityID, COMPONENT_TRANFORM,
                       entityComponentMasks);
  addComponentToEntity(playerEntityID, COMPONENT_MESH, entityComponentMasks);
  float xPos = 0.5f;
  float yPos = 0.25f;
  float width = 0.3f;
  float height = 0.1f;
  float rotation = 0.0f;
  setEntityTransform(ecs->transformComponent, playerEntityID, xPos, yPos, width,
                     height, rotation);
  float *vertices;

  // TODO: Move this to the rendering system
  // int vertArraySize =
  //     generateRectangleVertices(vertices, xPos, yPos, width, height);

  // // move this into rendering step
  // unsigned int VBO, VAO;
  // glGenVertexArrays(1, &VAO);
  // glGenBuffers(1, &VBO);

  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, vertArraySize * sizeof(float), vertices,
  //              GL_STATIC_DRAW);

  // glVertexAttribPointer(0, COMPONENTS_PER_VERTEX, GL_FLOAT, GL_FALSE,
  //                       COMPONENTS_PER_VERTEX * sizeof(float), (void *)0);
  // glEnableVertexAttribArray(0);

  // // Unbinds currently binded VBO/VAO
  // glBindBuffer(GL_ARRAY_BUFFER, 0);
  // glBindVertexArray(0);

  // setEntityMesh(ecs->meshComponent, playerEntityID, VAO, VBO, vertArraySize);
}
