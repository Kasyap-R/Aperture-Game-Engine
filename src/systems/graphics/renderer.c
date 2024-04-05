#include "renderer.h"
#include "../../../lib/stb_image.h"
#include "../../debug/debug.h"
#include "shaders.h"
#include "textures.h"
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <string.h>

#define PI 3.14159265358979323846

#define COMPONENTS_PER_TEXTURED_VERTEX 5
#define COMPONENTS_PER_COLORED_VERTEX 3
#define VERTICES_PER_RECTANGE 6

static HashMap *shaderTypeToID = NULL;

void render_LoadShaders() {
  u8 colorShaderID =
      compileAndLinkShaders("shaders/color.vert", "shaders/color.frag");
  u8 textureShaderID =
      compileAndLinkShaders("shaders/texture.vert", "shaders/texture.frag");
  ds_insertHashMap(&shaderTypeToID, SHADER_COLORED, colorShaderID);
  ds_insertHashMap(&shaderTypeToID, SHADER_TEXTURED, textureShaderID);
}

void render_RenderEntity(ECS *ecs, EntityID entityID) {
  u32 shaderProgramID = ecs->materialComponent->shaderProgramID[entityID];
  glUseProgram(shaderProgramID);
  // Calculating Translation Matrix
  {
    f32 xPos = ecs->transformComponent->x[entityID];
    f32 yPos = ecs->transformComponent->y[entityID];
    vec3 translationVector = {xPos, yPos, 0.0f};
    mat4 translationMatrix;
    glm_mat4_identity(translationMatrix);
    glm_translate(translationMatrix, translationVector);
    GLint transMatLocation = glGetUniformLocation(shaderProgramID, "uTransMat");
    glUniformMatrix4fv(transMatLocation, 1, GL_FALSE,
                       (const GLfloat *)translationMatrix);
  }
  // Setting up fragment shader
  {
    u32 textureID;
    ShaderType shaderType = ecs->materialComponent->shaderType[entityID];
    f32 rValue, gValue, bValue, aValue;
    switch (shaderType) {
    case SHADER_COLORED:
      rValue = ecs->colorComponent->rValue[entityID];
      gValue = ecs->colorComponent->gValue[entityID];
      bValue = ecs->colorComponent->bValue[entityID];
      aValue = ecs->colorComponent->aValue[entityID];
      setUniformVector4f(shaderProgramID, "uColor", rValue, gValue, bValue,
                         aValue);
      break;
    case SHADER_TEXTURED:
      textureID = ecs->spriteComponent->textureID[entityID];
      u32 texUniLoc = glGetUniformLocation(shaderProgramID, "uTexture");
      glUniform1i(texUniLoc, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textureID);
      break;
    default:
      printf("Invalid Shader Type\n");
      break;
    }
  }
  u32 VAO = ecs->meshComponent->VAO[entityID];
  u32 numVerts = ecs->meshComponent->vertexCount[entityID];
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, numVerts);
  glBindVertexArray(0);
}

void render_InstantiateRectangleEntity(ECS *ecs, EntityID entityID,
                                       ShaderType shaderType,
                                       ComponentMask *entityComponentMasks) {
  f32 *vertices_heap;
  u32 numVerts;
  u32 VAO, VBO;
  f32 width = ecs->transformComponent->xScale[entityID];
  f32 height = ecs->transformComponent->yScale[entityID];
  int vertArrayLength;
  u32 shaderProgramID;

  switch (shaderType) {
  case SHADER_COLORED:
    vertArrayLength =
        generateColoredRectangleVertices(&vertices_heap, width, height);
    shaderProgramID = ds_getHashMapValue(&shaderTypeToID, SHADER_COLORED);
    break;
  case SHADER_TEXTURED:
    vertArrayLength =
        generateTexturedRectangleVertices(&vertices_heap, width, height);
    shaderProgramID = ds_getHashMapValue(&shaderTypeToID, SHADER_TEXTURED);
    break;
  default:
    printf("Invalid Shader Type\n");
    exit(EXIT_FAILURE);
  }

  generateVertexBuffers(shaderType, vertices_heap, vertArrayLength, &VAO, &VBO,
                        &numVerts);

  setEntityMaterial(ecs->materialComponent, entityID, shaderProgramID,
                    shaderType);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, VERTICES_PER_RECTANGE);

  addComponentToEntity(ecs, entityID, COMPONENT_MESH, entityComponentMasks);
  addComponentToEntity(ecs, entityID, COMPONENT_MATERIAL, entityComponentMasks);

  free(vertices_heap);
}

void render_InstantiateCircleEntity(ECS *ecs, EntityID entityID,
                                    ShaderType shaderType,
                                    ComponentMask *entityComponentMasks) {
  f32 *vertices_heap;
  u32 numVerts;
  u32 VAO, VBO;
  f32 width = ecs->transformComponent->xScale[entityID];
  f32 height = ecs->transformComponent->yScale[entityID];
  int vertArrayLength;
  u32 shaderProgramID;

  switch (shaderType) {
  case SHADER_COLORED:
    vertArrayLength =
        generateColoredRectangleVertices(&vertices_heap, width, height);
    shaderProgramID = ds_getHashMapValue(&shaderTypeToID, SHADER_COLORED);
    break;
  case SHADER_TEXTURED:
    vertArrayLength =
        generateTexturedCircleVertices(&vertices_heap, width, height);
    shaderProgramID = ds_getHashMapValue(&shaderTypeToID, SHADER_TEXTURED);
    break;
  default:
    printf("Invalid Shader Type\n");
    exit(EXIT_FAILURE);
  }

  generateVertexBuffers(shaderType, vertices_heap, vertArrayLength, &VAO, &VBO,
                        &numVerts);

  setEntityMaterial(ecs->materialComponent, entityID, shaderProgramID,
                    shaderType);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO,
                vertArrayLength / COMPONENTS_PER_TEXTURED_VERTEX);

  addComponentToEntity(ecs, entityID, COMPONENT_MESH, entityComponentMasks);
  addComponentToEntity(ecs, entityID, COMPONENT_MATERIAL, entityComponentMasks);

  free(vertices_heap);
}

void render_AddSpriteComponent(ECS *ecs, EntityID entityID,
                               ComponentMask *entityComponentMasks,
                               char *texturePath) {
  u32 textureID = loadTexture(texturePath);
  setEntitySprite(ecs->spriteComponent, entityID, textureID);
  addComponentToEntity(ecs, entityID, COMPONENT_SPRITE, entityComponentMasks);
}

void render_AddColorComponent(ECS *ecs, EntityID entityID,
                              ComponentMask *entityComponentMasks, f32 rValue,
                              f32 gValue, f32 bValue, f32 aValue) {
  setEntityColor(ecs->colorComponent, entityID, rValue, gValue, bValue, aValue);
  addComponentToEntity(ecs, entityID, COMPONENT_COLOR, entityComponentMasks);
}

void generateVertexBuffers(ShaderType shaderType, f32 *vertices,
                           u32 vertArrayLength, u32 *VAO, u32 *VBO,
                           u32 *numVerts) {
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBindVertexArray(*VAO);
  glBufferData(GL_ARRAY_BUFFER, vertArrayLength * sizeof(f32), vertices,
               GL_STATIC_DRAW);

  {
    u32 posCoordsPerVertex = 3;
    u32 sizeOfVertex = sizeof(f32);
    u32 texCoordsPerVertex;
    u32 texCoordOffset;

    switch (shaderType) {
    case SHADER_COLORED:
      *numVerts = vertArrayLength / COMPONENTS_PER_COLORED_VERTEX;
      *numVerts *= COMPONENTS_PER_COLORED_VERTEX;
      sizeOfVertex *= COMPONENTS_PER_COLORED_VERTEX;
      glVertexAttribPointer(0, posCoordsPerVertex, GL_FLOAT, GL_FALSE,
                            sizeOfVertex, (void *)0);
      glEnableVertexAttribArray(0);
      break;
    case SHADER_TEXTURED:
      *numVerts = vertArrayLength / COMPONENTS_PER_TEXTURED_VERTEX;
      sizeOfVertex *= COMPONENTS_PER_TEXTURED_VERTEX;
      texCoordsPerVertex = 2;
      texCoordOffset = 3;
      glVertexAttribPointer(0, posCoordsPerVertex, GL_FLOAT, GL_FALSE,
                            sizeOfVertex, (void *)0);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(1, texCoordsPerVertex, GL_FLOAT, GL_FALSE,
                            sizeOfVertex,
                            (void *)(texCoordOffset * sizeof(f32)));
      glEnableVertexAttribArray(1);
      break;
    default:
      printf("Invalid Shader Type\n");
      exit(EXIT_FAILURE);
    }
  }
}

// TODO: Update this to support rotation when needed
i32 generateTexturedRectangleVertices(f32 **vertices, f32 width, f32 height) {
  f32 x = 0.0;
  f32 y = 0.0;
  f32 halfWidth = width / 2;
  f32 halfHeight = height / 2;
  i32 vertArrayLength = VERTICES_PER_RECTANGE * COMPONENTS_PER_TEXTURED_VERTEX;
  int vertArraySize = vertArrayLength * sizeof(f32);
  *vertices = (float *)malloc(vertArraySize);
  f32 rectangeVertices[] = {
      x - halfWidth, y + halfHeight, 0.0f, 0.0f, 1.0f, // Top-left
      x - halfWidth, y - halfHeight, 0.0f, 0.0f, 0.0f, // Bottom-left
      x + halfWidth, y - halfHeight, 0.0f, 1.0f, 0.0f, // Bottom-right

      x - halfWidth, y + halfHeight, 0.0f, 0.0f, 1.0f, // Top-left
      x + halfWidth, y - halfHeight, 0.0f, 1.0f, 0.0f, // Bottom-right
      x + halfWidth, y + halfHeight, 0.0f, 1.0f, 1.0f, // Top-right
  };
  memcpy(*vertices, rectangeVertices, vertArraySize);
  return vertArrayLength;
}

i32 generateTexturedCircleVertices(f32 **vertices, f32 width, f32 height) {
  i32 segments = 30;
  f32 radius = width / 2;
  i32 verticesPerSegment = 3;
  i32 vertArrayLength =
      segments * verticesPerSegment * COMPONENTS_PER_TEXTURED_VERTEX;

  *vertices = (float *)malloc(vertArrayLength * sizeof(float));
  for (int i = 0; i < segments; ++i) {
    f32 angle1 = 2 * PI * (float)i / segments;
    f32 angle2 = 2 * PI * (float)(i + 1) / segments;

    int baseIndex = i * verticesPerSegment * COMPONENTS_PER_TEXTURED_VERTEX;
    (*vertices)[baseIndex + 0] = 0.0f; // x
    (*vertices)[baseIndex + 1] = 0.0f; // y
    (*vertices)[baseIndex + 2] = 0.0f; // z
    (*vertices)[baseIndex + 3] = 0.5f; // u (texture x)
    (*vertices)[baseIndex + 4] = 0.5f; // v (texture y)
    // First vertex on circumference
    (*vertices)[baseIndex + 5] = radius * cos(angle1);        // x
    (*vertices)[baseIndex + 6] = radius * sin(angle1);        // y
    (*vertices)[baseIndex + 7] = 0.0f;                        // z
    (*vertices)[baseIndex + 8] = (cos(angle1) + 1.0f) / 2.0f; // u
    (*vertices)[baseIndex + 9] = (sin(angle1) + 1.0f) / 2.0f; // v

    // Second vertex on circumference
    (*vertices)[baseIndex + 10] = radius * cos(angle2);        // x
    (*vertices)[baseIndex + 11] = radius * sin(angle2);        // y
    (*vertices)[baseIndex + 12] = 0.0f;                        // z
    (*vertices)[baseIndex + 13] = (cos(angle2) + 1.0f) / 2.0f; // u
    (*vertices)[baseIndex + 14] = (sin(angle2) + 1.0f) / 2.0f; // v
  }
  return vertArrayLength;
}

// TODO: Update this to support rotation when needed
i32 generateColoredRectangleVertices(f32 **vertices, f32 width, f32 height) {
  f32 x = 0.0;
  f32 y = 0.0;
  f32 halfWidth = width / 2;
  f32 halfHeight = height / 2;
  i32 vertArrayLength = VERTICES_PER_RECTANGE * COMPONENTS_PER_COLORED_VERTEX;
  int vertArraySize = vertArrayLength * sizeof(f32);
  *vertices = (float *)malloc(vertArraySize);
  f32 rectangeVertices[] = {
      x - halfWidth, y + halfHeight, 0.0f, // Top-left
      x - halfWidth, y - halfHeight, 0.0f, // Bottom-left
      x + halfWidth, y - halfHeight, 0.0f, // Bottom-right

      x - halfWidth, y + halfHeight, 0.0f, // Top-left
      x + halfWidth, y - halfHeight, 0.0f, // Bottom-right
      x + halfWidth, y + halfHeight, 0.0f, // Top-right
  };
  memcpy(*vertices, rectangeVertices, vertArraySize);
  return vertArrayLength;
}
