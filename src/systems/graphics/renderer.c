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

#define COMPONENTS_PER_VERTEX 5
#define VERTICES_PER_RECTANGE 6

static HashMap *shaderTypeToID = NULL;

void render_RenderComponent(ECS *ecs, EntityID entityID) {
  u32 shaderProgramID = ecs->materialComponent->shaderProgramID[entityID];
  glUseProgram(shaderProgramID);
  u32 textureID = ecs->spriteComponent->textureID[entityID];
  // Calculate translation matrix (part of model matrix)
  {
    f32 xTargetPos = ecs->transformComponent->x[entityID];
    f32 yTargetPos = ecs->transformComponent->y[entityID];
    vec3 translation = {xTargetPos, yTargetPos, 0.0f};
    mat4 translationMatrix;
    glm_mat4_identity(translationMatrix); // start with identity matrix
    glm_translate(translationMatrix, translation);
    GLint transMatLocation = glGetUniformLocation(shaderProgramID, "uTransMat");
    glUniformMatrix4fv(transMatLocation, 1, GL_FALSE,
                       (const GLfloat *)translationMatrix);
  }
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
  }

  u32 VAO = ecs->meshComponent->VAO[entityID];
  u32 numVerts = ecs->meshComponent->vertexCount[entityID];
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, numVerts);
  glBindVertexArray(0);
}

void render_InstantiatePlayerEntity(ECS *ecs, EntityID entityID,
                                    ComponentMask *entityComponentMasks) {
  // Currently assumign array size, should prob change in future
  i32 vertArraySize = VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX;
  f32 *vertices;
  u32 VAO, VBO;
  u32 textureID;

  // Initialize VAO/VBO
  setupRectangleGeometry(ecs->transformComponent, entityID, vertices, &VAO,
                         &VBO);

  u32 shaderProgramID =
      compileAndLinkShaders("shaders/player.vert", "shaders/player.frag");

  textureID = loadTexture("textures/platform_texture.png");

  setEntityMaterial(ecs->materialComponent, entityID, shaderProgramID, 0.29,
                    0.0, 0.51, 1.0, SHADER_TEXTURED);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, VERTICES_PER_RECTANGE);
  setEntitySprite(ecs->spriteComponent, entityID, textureID);

  addComponentToEntity(ecs, entityID, COMPONENT_MESH, entityComponentMasks);
  addComponentToEntity(ecs, entityID, COMPONENT_MATERIAL, entityComponentMasks);
  addComponentToEntity(ecs, entityID, COMPONENT_SPRITE, entityComponentMasks);
}

void render_LoadShaders() {
  u8 colorShaderID =
      compileAndLinkShaders("shaders/color.vert", "shaders/color.frag");
  u8 textureShaderID =
      compileAndLinkShaders("shaders/texture.vert", "shaders/texture.frag");
  ds_insertHashMap(&shaderTypeToID, SHADER_COLORED, colorShaderID);
  ds_insertHashMap(&shaderTypeToID, SHADER_TEXTURED, textureShaderID);
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
  u32 textureID;

  printf("Shader Type: %d\n", shaderType);

  switch (shaderType) {
  case SHADER_COLORED:
    break;
  case SHADER_TEXTURED:
    vertArrayLength = generateRectangleVertices(&vertices_heap, width, height);
    setupRectangleGeometry(ecs->transformComponent, entityID, vertices_heap,
                           &VAO, &VBO);
    shaderProgramID =
        compileAndLinkShaders("shaders/player.vert", "shaders/player.frag");
    break;
  default:
    printf("Invalid Shader Type\n");
    exit(EXIT_FAILURE);
  }

  setEntityMaterial(ecs->materialComponent, entityID, shaderProgramID, 0.29,
                    0.0, 0.51, 1.0, SHADER_TEXTURED);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, VERTICES_PER_RECTANGE);

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

void setupRectangleGeometry(TransformComponent *transformComponent,
                            EntityID entityID, f32 *vertices, u32 *VAO,
                            u32 *VBO) {

  u32 vertArrayLength = COMPONENTS_PER_VERTEX * VERTICES_PER_RECTANGE;
  f32 posCoordsPerVertex = 3;
  f32 texCoordsPerVertex = 2;
  i32 sizeOfVertex = COMPONENTS_PER_VERTEX * sizeof(f32);
  u8 texCoordOffset = 3;
  // Set up VBO/VAO with data and data format respectively
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBindVertexArray(*VAO);
  glBufferData(GL_ARRAY_BUFFER, vertArrayLength * sizeof(f32), vertices,
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, posCoordsPerVertex, GL_FLOAT, GL_FALSE, sizeOfVertex,
                        (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, texCoordsPerVertex, GL_FLOAT, GL_FALSE, sizeOfVertex,
                        (void *)(texCoordOffset * sizeof(f32)));
  glEnableVertexAttribArray(1);
}

// TODO: Update this to support rotation when needed
i32 generateRectangleVertices(f32 **vertices, f32 width, f32 height) {
  f32 x = 0.0;
  f32 y = 0.0;
  f32 halfWidth = width / 2;
  f32 halfHeight = height / 2;
  f32 rectangeVertices[] = {
      x - halfWidth, y + halfHeight, 0.0f, 0.0f, 1.0f, // Top-left
      x - halfWidth, y - halfHeight, 0.0f, 0.0f, 0.0f, // Bottom-left
      x + halfWidth, y - halfHeight, 0.0f, 1.0f, 0.0f, // Bottom-right

      x - halfWidth, y + halfHeight, 0.0f, 0.0f, 1.0f, // Top-left
      x + halfWidth, y - halfHeight, 0.0f, 1.0f, 0.0f, // Bottom-right
      x + halfWidth, y + halfHeight, 0.0f, 1.0f, 1.0f, // Top-right
  };
  i32 vertArraySize = VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX;
  *vertices = malloc(vertArraySize * sizeof(f32));
  memcpy(*vertices, rectangeVertices, vertArraySize * sizeof(f32));
  return vertArraySize;
}
