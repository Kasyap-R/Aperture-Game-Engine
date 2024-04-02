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

static i32 textureID;

void render_RenderComponent(ECS *ecs, EntityID entityID) {
  u32 shaderProgramID = ecs->materialComponent->shaderProgramID[entityID];
  glUseProgram(shaderProgramID);
  // Calculate translation matrix (part of model matrix)
  {
    f32 xTargetPos = ecs->transformComponent->x[entityID];
    f32 yTargetPos = ecs->transformComponent->y[entityID];
    vec3 translation = {xTargetPos, yTargetPos, 0.0f};
    mat4 translationMatrix;
    glm_mat4_identity(translationMatrix); // start with identity matrix
    glm_translate(translationMatrix, translation);
    GLint transMatLocation = glGetUniformLocation(shaderProgramID, "uTransMat");
    // This sets the initial position to the 2 * starting position as I
    // translate it by this amount I could solve this by setting the initial
    // value and subtracting from that or I could just set up a better system
    // where I actually define a coordinate system and accurately convert that
    // to NDC
    glUniformMatrix4fv(transMatLocation, 1, GL_FALSE,
                       (const GLfloat *)translationMatrix);
  }
  {
    /* f32 rValue = ecs->materialComponent->rValue[entityID];
    f32 gValue = ecs->materialComponent->gValue[entityID];
    f32 bValue = ecs->materialComponent->bValue[entityID];
    f32 aValue = ecs->materialComponent->aValue[entityID]; */
    // Only applicable to entities who are of the shader type "SHADER_COLORED"
    setUniformVector4f(shaderProgramID, "uColor", 0.29, 0.0, 0.1, 1.0);
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
  f32 vertices[vertArraySize];
  u32 VAO, VBO;

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

void setupRectangleGeometry(TransformComponent *transformComponent,
                            EntityID entityID, f32 *vertices, u32 *VAO,
                            u32 *VBO) {
  i32 vertArraySize;
  {
    f32 xPos = transformComponent->x[entityID];
    f32 yPos = transformComponent->y[entityID];
    f32 width = transformComponent->xScale[entityID];
    f32 height = transformComponent->yScale[entityID];

    vertArraySize =
        generateRectangleVertices(vertices, xPos, yPos, width, height);
  }
  {
    f32 posCoordsPerVertex = 3;
    f32 texCoordsPerVertex = 2;
    i32 sizeOfVertex = COMPONENTS_PER_VERTEX * sizeof(f32);
    u8 texCoordOffset = 3;
    // Set up VBO/VAO with data and data format respectively
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBindVertexArray(*VAO);
    glBufferData(GL_ARRAY_BUFFER, vertArraySize * sizeof(f32), vertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, posCoordsPerVertex, GL_FLOAT, GL_FALSE,
                          sizeOfVertex, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, texCoordsPerVertex, GL_FLOAT, GL_FALSE,
                          sizeOfVertex, (void *)(texCoordOffset * sizeof(f32)));
    glEnableVertexAttribArray(1);
  }
}

// TODO: Update this to support rotation when needed
i32 generateRectangleVertices(f32 *vertices, f32 x, f32 y, f32 width,
                              f32 height) {
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
  memcpy(vertices, rectangeVertices, vertArraySize * sizeof(f32));
  return vertArraySize;
}
