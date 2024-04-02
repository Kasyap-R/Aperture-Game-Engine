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

static int textureID;

void render_RenderComponent(ECS *ecs, uint8_t entityID) {
  unsigned int shaderProgramID =
      ecs->materialComponent->shaderProgramID[entityID];
  glUseProgram(shaderProgramID);
  // Calculate translation matrix (part of model matrix)
  {
    float xTargetPos = ecs->transformComponent->x[entityID];
    float yTargetPos = ecs->transformComponent->y[entityID];
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
    /* float rValue = ecs->materialComponent->rValue[entityID];
    float gValue = ecs->materialComponent->gValue[entityID];
    float bValue = ecs->materialComponent->bValue[entityID];
    float aValue = ecs->materialComponent->aValue[entityID]; */
    // Only applicable to entities who are of the shader type "SHADER_COLORED"
    setUniformVector4f(shaderProgramID, "uColor", 0.29, 0.0, 0.1, 1.0);
  }
  {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
  }

  unsigned int VAO = ecs->meshComponent->VAO[entityID];
  unsigned int numVerts = ecs->meshComponent->vertexCount[entityID];
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, numVerts);
  glBindVertexArray(0);
}

void render_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                    ComponentMask *entityComponentMasks) {
  // Currently assumign array size, should prob change in future
  int vertArraySize = VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX;
  float vertices[vertArraySize];
  unsigned int VAO, VBO;

  // Initialize VAO/VBO
  setupRectangleGeometry(ecs->transformComponent, entityID, vertices, &VAO,
                         &VBO);

  unsigned int shaderProgramID =
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
                            uint8_t entityID, float *vertices,
                            unsigned int *VAO, unsigned int *VBO) {
  int vertArraySize;
  {
    float xPos = transformComponent->x[entityID];
    float yPos = transformComponent->y[entityID];
    float width = transformComponent->xScale[entityID];
    float height = transformComponent->yScale[entityID];

    vertArraySize =
        generateRectangleVertices(vertices, xPos, yPos, width, height);
  }
  {
    float posCoordsPerVertex = 3;
    float texCoordsPerVertex = 2;
    int sizeOfVertex = COMPONENTS_PER_VERTEX * sizeof(float);
    uint8_t texCoordOffset = 3;
    // Set up VBO/VAO with data and data format respectively
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBindVertexArray(*VAO);
    glBufferData(GL_ARRAY_BUFFER, vertArraySize * sizeof(float), vertices,
                 GL_STATIC_DRAW);
    glVertexAttribPointer(0, posCoordsPerVertex, GL_FLOAT, GL_FALSE,
                          sizeOfVertex, (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, texCoordsPerVertex, GL_FLOAT, GL_FALSE,
                          sizeOfVertex,
                          (void *)(texCoordOffset * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
}

// TODO: Update this to support rotation when needed
int generateRectangleVertices(float *vertices, float x, float y, float width,
                              float height) {
  float halfWidth = width / 2;
  float halfHeight = height / 2;
  float rectangeVertices[] = {
      x - halfWidth, y + halfHeight, 0.0f, 0.0f, 1.0f, // Top-left
      x - halfWidth, y - halfHeight, 0.0f, 0.0f, 0.0f, // Bottom-left
      x + halfWidth, y - halfHeight, 0.0f, 1.0f, 0.0f, // Bottom-right

      x - halfWidth, y + halfHeight, 0.0f, 0.0f, 1.0f, // Top-left
      x + halfWidth, y - halfHeight, 0.0f, 1.0f, 0.0f, // Bottom-right
      x + halfWidth, y + halfHeight, 0.0f, 1.0f, 1.0f, // Top-right
  };
  int vertArraySize = VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX;
  memcpy(vertices, rectangeVertices, vertArraySize * sizeof(float));
  return vertArraySize;
}
