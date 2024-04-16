#include "shapes.h"

#include <stdlib.h>
#include <string.h>

// TODO: Support Quaternions
u32 create_colored_rec_prism_vertices(f32 **vertices, f32 xScale, f32 yScale,
                                      f32 zScale) {
  f32 halfX = xScale / 2.0f;
  f32 halfY = yScale / 2.0f;
  f32 halfZ = zScale / 2.0f;
  i32 vertArrayLength =
      UNIQUE_VERTICES_PER_RECTANGULAR_PRISM * COMPONENTS_PER_COLORED_VERTEX;
  int vertArraySize = vertArrayLength * sizeof(f32);
  *vertices = (float *)malloc(vertArraySize);
  f32 prismVerts[] = {
      // Front face
      halfX, halfY, halfZ,   // Top-Right
      -halfX, halfY, halfZ,  // Top-Left
      -halfX, -halfY, halfZ, // Bottom-Left
      halfX, -halfY, halfZ,  // Bottom-Right

      // Back face
      halfX, halfY, -halfZ,   // Top-Right
      -halfX, halfY, -halfZ,  // Top-Left
      -halfX, -halfY, -halfZ, // Bottom-Left
      halfX, -halfY, -halfZ   // Bottom-Right
  };
  memcpy(*vertices, prismVerts, vertArraySize);
  return vertArraySize;
}

u32 create_textured_rec_prism_vertices(f32 **vertices, f32 xScale, f32 yScale,
                                       f32 zScale) {
  f32 halfX = xScale / 2.0f;
  f32 halfY = yScale / 2.0f;
  f32 halfZ = zScale / 2.0f;
  i32 vertArrayLength =
      UNIQUE_VERTICES_PER_RECTANGULAR_PRISM * COMPONENTS_PER_TEXTURED_VERTEX;
  int vertArraySize = vertArrayLength * sizeof(f32);
  *vertices = (float *)malloc(vertArraySize);
  f32 prismVerts[] = {
      // Front face
      halfX, halfY, halfZ, 1.0f, 1.0f,   // Top-Right
      -halfX, halfY, halfZ, 0.0f, 1.0f,  // Top-Left
      -halfX, -halfY, halfZ, 0.0f, 0.0f, // Bottom-Left
      halfX, -halfY, halfZ, 1.0f, 0.0f,  // Bottom-Right

      // Back face
      halfX, halfY, -halfZ, 0.0f, 1.0f,   // Top-Right
      -halfX, halfY, -halfZ, 1.0f, 1.0f,  // Top-Left (note the reversal)
      -halfX, -halfY, -halfZ, 1.0f, 0.0f, // Bottom-Left
      halfX, -halfY, -halfZ, 0.0f, 0.0f,  // Bottom-Right

  };

  memcpy(*vertices, prismVerts, vertArraySize);
  return vertArraySize;
}
