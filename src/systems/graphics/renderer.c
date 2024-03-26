#include "renderer.h"

#define COMPONENTS_PER_VERTEX 3
#define VERTICES_PER_RECTANGE 6

void render_RenderComponent(ECS *ecs, uint8_t entityID) {
  // We modify VBO based upon the transform
}

void render_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                    ComponentMask *entityComponentMasks) {
  addComponentToEntity(entityID, COMPONENT_MESH, entityComponentMasks);
  // I assume the size of the vertices array here, must be fixed later
  float vertices[VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX];
  float xPos = ecs->transformComponent->x[entityID];
  float yPos = ecs->transformComponent->x[entityID];
  float width = ecs->transformComponent->xScale[entityID];
  float height = ecs->transformComponent->yScale[entityID];

  int vertArraySize =
      generateRectangleVertices(vertices, xPos, yPos, width, height);

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertArraySize * sizeof(float), &vertices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, COMPONENTS_PER_VERTEX, GL_FLOAT, GL_FALSE,
                        COMPONENTS_PER_VERTEX * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Unbinds currently binded VBO/VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, vertArraySize);
}

// TODO: Update this to support rotation when needed
int generateRectangleVertices(float *vertices, float x, float y, float width,
                              float height) {
  float halfWidth = width / 2;
  float halfHeight = height / 2;
  float rectangeVertices[] = {
      x - halfWidth, y + halfHeight, 0.0f, // Top-left
      x - halfWidth, y - halfHeight, 0.0f, // Bottom-left
      x + halfWidth, y - halfHeight, 0.0f, // Bottom-right

      x - halfWidth, y + halfHeight, 0.0f, // Top-left
      x + halfWidth, y - halfHeight, 0.0f, // Bottom-right
      x + halfWidth, y + halfHeight, 0.0f  // Top-right
  };

  int vertArraySize = VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX;
  memcpy(vertices, rectangeVertices, vertArraySize * sizeof(float));
  return vertArraySize;
}
