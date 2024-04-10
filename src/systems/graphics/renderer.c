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

#define COMPONENTS_PER_TEXTURED_VERTEX 5
#define COMPONENTS_PER_COLORED_VERTEX 3
#define VERTICES_PER_RECTANGE 6

static HashMap *shaderTypeToID = NULL;
static const f32 aspectRatio = (1920.0f / 1080.0f);

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
  // Setting up transformation matrix
  {
    f32 xPos = ecs->transformComponent->x[entityID];
    f32 yPos = ecs->transformComponent->y[entityID];
    f32 width = ecs->transformComponent->xScale[entityID];
    vec3 translationVector = {xPos, yPos, 0.0f};
    mat4 translationMatrix;
    glm_mat4_identity(translationMatrix);
    glm_translate(translationMatrix, translationVector);

    // Projection Matrix
    float left, right, bottom, top, nearZ, farZ;
    nearZ = -1.0f;
    farZ = 1.0f;
    // Set these values to match up with our planned coordinate system and also
    // support a 16:9 aspect ratio
    left = -1000.0;
    right = 1000.0;
    bottom = -562.5;
    top = 562.5;
    mat4 projectionMatrix;
    glm_mat4_identity(projectionMatrix);
    glm_ortho(left, right, bottom, top, nearZ, farZ, projectionMatrix);

    mat4 transformationMatrix;
    glm_mat4_mul(projectionMatrix, translationMatrix, transformationMatrix);
    GLint transMatLocation = glGetUniformLocation(shaderProgramID, "uTransMat");
    glUniformMatrix4fv(transMatLocation, 1, GL_FALSE,
                       (const GLfloat *)transformationMatrix);
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
  glDrawArrays(GL_TRIANGLE_FAN, 0, numVerts);
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
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, numVerts);

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
  f32 radius = width / 2;

  u32 shaderProgramID = ds_getHashMapValue(&shaderTypeToID, SHADER_COLORED);

  switch (shaderType) {
  case SHADER_COLORED:
    vertArrayLength = generateColoredCircleVertices(&vertices_heap, height / 2);
    break;
  case SHADER_TEXTURED:
    vertArrayLength =
        generateTexturedCircleVertices(&vertices_heap, height / 2);
    break;
  default:
    printf("Invalid Shader Type\n");
    exit(EXIT_FAILURE);
  }

  generateVertexBuffers(shaderType, vertices_heap, vertArrayLength, &VAO, &VBO,
                        &numVerts);

  setEntityMaterial(ecs->materialComponent, entityID, shaderProgramID,
                    shaderType);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, numVerts);

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

i32 generateColoredCircleVertices(f32 **vertices, f32 radius) {
  i32 numTrianges = 30;
  // 3 Vertices for the first triangle and one additional one for each one after
  i32 numVertices = numTrianges + 2;
  f32 doublePI = 2.0f * M_PI;
  i32 vertArrayLength = numVertices * COMPONENTS_PER_COLORED_VERTEX;

  *vertices = (f32 *)malloc(vertArrayLength * sizeof(f32));
  // Definining central vertex
  (*vertices)[0] = 0.0f; // x
  (*vertices)[1] = 0.0f; // y
  (*vertices)[2] = 0.0f; // z

  for (i32 i = 1; i <= numTrianges; i++) {
    i32 currVertexIndex = COMPONENTS_PER_COLORED_VERTEX * i;
    (*vertices)[currVertexIndex] =
        radius * cos(i * doublePI / (float)numTrianges);
    (*vertices)[currVertexIndex + 1] =
        radius * sin(i * doublePI / (float)numTrianges);
    (*vertices)[currVertexIndex + 2] = 0.0f;
  }

  // Defining Last Vertex - same as first perimeter vertex
  i32 lastVertexIndex = COMPONENTS_PER_COLORED_VERTEX * (numTrianges + 1);
  (*vertices)[lastVertexIndex] = radius * cos(doublePI / numTrianges);
  (*vertices)[lastVertexIndex + 1] = radius * sin(doublePI / numTrianges);
  (*vertices)[lastVertexIndex + 2] = 0.0f;
  return vertArrayLength;
}

// Using the triangle fan method; We start with a central vertex and build
// triangles off of it
i32 generateTexturedCircleVertices(f32 **vertices, f32 radius) {
  i32 numTrianges = 30;
  // 3 Vertices for the first triangle and one additional one for each one after
  i32 numVertices = numTrianges + 2;
  f32 doublePI = 2.0f * M_PI;
  i32 vertArrayLength = numVertices * COMPONENTS_PER_TEXTURED_VERTEX;

  *vertices = (f32 *)malloc(vertArrayLength * sizeof(f32));
  // Definining central vertex
  (*vertices)[0] = 0.0f; // x
  (*vertices)[1] = 0.0f; // y
  (*vertices)[2] = 0.0f; // z
  (*vertices)[3] = 0.5f; // u (texture x)
  (*vertices)[4] = 0.5f; // v (texture y)

  for (i32 i = 1; i < numVertices; i++) {
    i32 currVertexIndex = 5 * i;
    (*vertices)[currVertexIndex] = radius * cos(i * doublePI / numTrianges);
    (*vertices)[currVertexIndex + 1] = radius * sin(i * doublePI / numTrianges);
    (*vertices)[currVertexIndex + 2] = 0.0f;
    (*vertices)[currVertexIndex + 3] = 0.0f;
    (*vertices)[currVertexIndex + 4] = 0.0f;
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
