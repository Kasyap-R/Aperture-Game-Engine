#include "renderer.h"

#define COMPONENTS_PER_VERTEX 3

void renderPlayer(ECS *ecs, uint8_t entityID) {}

// TODO: Update this to support rotation when needed
int generateRectangleVertices(float *vertices, float x, float y, float width,
                              float height) {
  const int verticesPerRectange = 6; // Two triangles
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

  int vertArraySize = verticesPerRectange * COMPONENTS_PER_VERTEX;
  memcpy(vertices, rectangeVertices, vertArraySize * sizeof(float));
  return vertArraySize;
}