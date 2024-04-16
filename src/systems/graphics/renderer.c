#include "renderer.h"
#include "../../../lib/stb_image.h"
#include "../../debug/debug.h"
#include "shaders.h"
#include "shapes.h"
#include "textures.h"
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <string.h>

static HashMap *shaderTypeToID = NULL;
static const f32 aspectRatio = (1920.0f / 1080.0f);
static mat4 viewMatrix;
static mat4 projectionMatrix;

void render_LoadShaders() {
  u8 colorShaderID =
      compileAndLinkShaders("shaders/color.vert", "shaders/color.frag");
  u8 textureShaderID =
      compileAndLinkShaders("shaders/texture.vert", "shaders/texture.frag");
  ds_insertHashMap(&shaderTypeToID, SHADER_COLORED, colorShaderID);
  ds_insertHashMap(&shaderTypeToID, SHADER_TEXTURED, textureShaderID);
}

void render_init_projection_matrix() {
  float fov, nearZ, farZ;
  nearZ = 0.1f;
  farZ = 500.0f;
  fov = 90.0f;
  glm_mat4_identity(projectionMatrix);
  glm_perspective(glm_rad(fov), aspectRatio, nearZ, farZ, projectionMatrix);
}

void render_RenderEntity(ECS *ecs, EntityID entityID) {
  u32 shaderProgramID = ecs->materialComponent->shaderProgramID[entityID];
  glUseProgram(shaderProgramID);
  // Setting up transformation matrix
  {
    // Translation Matrix
    f32 xPos = ecs->transformComponent->x[entityID];
    f32 yPos = ecs->transformComponent->y[entityID];
    f32 zPos = ecs->transformComponent->z[entityID];
    vec3 translationVector = {xPos, yPos, zPos};
    mat4 translationMatrix;
    glm_mat4_identity(translationMatrix);
    glm_translate(translationMatrix, translationVector);

    mat4 modelViewMatrix;
    glm_mat4_mul(viewMatrix, translationMatrix, modelViewMatrix);
    mat4 transformationMatrix;
    glm_mat4_mul(projectionMatrix, modelViewMatrix, transformationMatrix);
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
  draw_elements(ecs, entityID);
}

void render_calculate_view_matrix(ECS *ecs, EntityID cameraID) {
  f32 xPos = ecs->transformComponent->x[cameraID];
  f32 yPos = ecs->transformComponent->y[cameraID];
  f32 zPos = ecs->transformComponent->z[cameraID];

  vec3 cameraPos = {xPos, yPos, zPos};
  vec3 cameraTarget = {0.0f, 0.0f, 0.0f};
  vec3 upVector = {0.0f, 1.0f, 0.0f};
  glm_lookat(cameraPos, cameraTarget, upVector, viewMatrix);
}

void draw_elements(ECS *ecs, EntityID entityID) {
  u32 VAO = ecs->meshComponent->VAO[entityID];
  u32 numVerts = ecs->meshComponent->vertexCount[entityID];
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, numVerts, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void render_init_rec_prism(ECS *ecs, EntityID entityID, ShaderType shaderType,
                           ComponentMask *entityComponentMasks) {
  f32 *vertices_heap;
  u32 numVerts;
  u32 VAO, VBO, EBO;
  f32 xScale = ecs->transformComponent->xScale[entityID];
  f32 yScale = ecs->transformComponent->yScale[entityID];
  f32 zScale = ecs->transformComponent->zScale[entityID];

  u32 vertArraySize;
  u32 shaderProgramID;

  switch (shaderType) {
  case SHADER_COLORED:
    vertArraySize = create_colored_rec_prism_vertices(&vertices_heap, xScale,
                                                      yScale, zScale);
    shaderProgramID = ds_getHashMapValue(&shaderTypeToID, SHADER_COLORED);
    break;
  case SHADER_TEXTURED:
    vertArraySize = 40;
    shaderProgramID = ds_getHashMapValue(&shaderTypeToID, SHADER_TEXTURED);
    break;
  default:
    printf("Invalid Shader Type\n");
    exit(EXIT_FAILURE);
  }

  generateVertexBuffers(shaderType, vertices_heap, vertArraySize, &VAO, &VBO);
  // Red TODO: add support for shape types later; currently assuming only rec
  // prisms
  init_rec_prism_EBO(&VAO, &EBO);
  numVerts = sizeof(rec_prism_indices) / sizeof(rec_prism_indices[0]);

  setEntityMaterial(ecs->materialComponent, entityID, shaderProgramID,
                    shaderType);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, EBO, numVerts);
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
                           u32 vertArraySize, u32 *VAO, u32 *VBO) {
  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBindVertexArray(*VAO);
  glBufferData(GL_ARRAY_BUFFER, vertArraySize, vertices, GL_STATIC_DRAW);

  {
    switch (shaderType) {
    case SHADER_COLORED:
      init_colored_VAO();
      break;
    case SHADER_TEXTURED:
      init_textured_VAO();
      break;
    default:
      printf("Invalid Shader Type\n");
      exit(EXIT_FAILURE);
    }
  }
}

void init_colored_VAO() {
  u32 sizeOfVertex = sizeof(f32) * COMPONENTS_PER_COLORED_VERTEX;
  u32 posCoordsPerVertex = 3;
  glVertexAttribPointer(0, posCoordsPerVertex, GL_FLOAT, GL_FALSE, sizeOfVertex,
                        (void *)0);
  glEnableVertexAttribArray(0);
}

void init_textured_VAO() {
  u32 sizeOfVertex = sizeof(f32) * COMPONENTS_PER_TEXTURED_VERTEX;
  u32 posCoordsPerVertex = 3;
  u32 texCoordsPerVertex = 2;
  u32 texCoordOffset = 3;
  glVertexAttribPointer(0, posCoordsPerVertex, GL_FLOAT, GL_FALSE, sizeOfVertex,
                        (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, texCoordsPerVertex, GL_FLOAT, GL_FALSE, sizeOfVertex,
                        (void *)(texCoordOffset * sizeof(f32)));
  glEnableVertexAttribArray(1);
}

void init_rec_prism_EBO(u32 *VAO, u32 *EBO) {
  glBindVertexArray(*VAO);
  glGenBuffers(1, EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rec_prism_indices),
               rec_prism_indices, GL_STATIC_DRAW);
}
