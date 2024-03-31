#include "renderer.h"
#include "shaders.h"
#include <cglm/affine.h>
#include <cglm/cam.h>
#include <cglm/mat4.h>
#include <cglm/types.h>
#include <string.h>

#define COMPONENTS_PER_VERTEX 3
#define VERTICES_PER_RECTANGE 6

void render_RenderComponent(ECS *ecs, uint8_t entityID) {
  unsigned int shaderProgramID =
      ecs->materialComponent->shaderProgramID[entityID];
  glUseProgram(shaderProgramID);
  // Calculate translation matrix (part of model matrix)
  float xTargetPos = ecs->transformComponent->x[entityID];
  float yTargetPos = ecs->transformComponent->y[entityID];
  float rValue = ecs->materialComponent->rValue[entityID];
  float gValue = ecs->materialComponent->gValue[entityID];
  float bValue = ecs->materialComponent->bValue[entityID];
  float aValue = ecs->materialComponent->aValue[entityID];
  // This sets the initial position to the 2 * starting position as I translate
  // it by this amount I could solve this by setting the initial value and
  // subtracting from that or I could just set up a better system where I
  // actually define a coordinate system and accurately convert that to NDC
  vec3 translation = {xTargetPos, yTargetPos, 0.0f};
  mat4 translationMatrix;
  glm_mat4_identity(translationMatrix); // start with identity matrix
  glm_translate(translationMatrix, translation);
  GLint transMatLocation = glGetUniformLocation(shaderProgramID, "uTransMat");
  glUniformMatrix4fv(transMatLocation, 1, GL_FALSE,
                     (const GLfloat *)translationMatrix);
  // Set color
  setUniform4f(shaderProgramID, "uColor", rValue, gValue, bValue, aValue);
  // Define view matrix
  mat4 viewMatrix;
  glm_mat4_identity(viewMatrix);

  // Define Prjection Matrix
  mat4 projectionMatrix;

  unsigned int VAO = ecs->meshComponent->VAO[entityID];
  unsigned int numVerts = ecs->meshComponent->vertexCount[entityID];
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, numVerts);
  glBindVertexArray(0);
}

void render_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                    ComponentMask *entityComponentMasks) {
  // I assume the size of the vertices array here, must be fixed later
  float vertices[VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX];
  unsigned int VBO;
  unsigned int VAO;
  float xPos = ecs->transformComponent->x[entityID];
  float yPos = ecs->transformComponent->y[entityID];
  float width = ecs->transformComponent->xScale[entityID];
  float height = ecs->transformComponent->yScale[entityID];
  int vertArraySize =
      generateRectangleVertices(vertices, xPos, yPos, width, height);

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);
  glBufferData(GL_ARRAY_BUFFER, vertArraySize * sizeof(float), &vertices,
               GL_STATIC_DRAW);
  // We tell OpenGL how to interpret the data per vertex; each vertex is 3
  // values
  // The first argument sets the location of the vertex attribute to 0 which is
  // reflected by our shader setting "layout (location = 0)"
  glVertexAttribPointer(0, COMPONENTS_PER_VERTEX, GL_FLOAT, GL_FALSE,
                        COMPONENTS_PER_VERTEX * sizeof(float), (void *)0);
  // We enable that array, giving the location as the argument
  glEnableVertexAttribArray(0);
  // Unbinds currently binded VBO/VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned int shaderProgramID =
      compileAndLinkShaders("shaders/player.vert", "shaders/player.frag");
  setEntityMaterial(ecs->materialComponent, entityID, shaderProgramID, 0.29,
                    0.0, 0.51, 1.0);

  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, VERTICES_PER_RECTANGE);
  addComponentToEntity(ecs, entityID, COMPONENT_MESH, entityComponentMasks);
  addComponentToEntity(ecs, entityID, COMPONENT_MATERIAL, entityComponentMasks);
}

// Currently doesn't work and produces weird shapes. Look into it later when you
// actually need to have a function to do this
// >>>>>>>>>>>>>UNUSED FUNCTION<<<<<<<<<<<< Red
void setupRectangleGeometry(ECS *ecs, uint8_t entityID, float *vertices,
                            unsigned int *VAO, unsigned int *VBO) {
  float xPos = ecs->transformComponent->x[entityID];
  float yPos = ecs->transformComponent->y[entityID];
  float width = ecs->transformComponent->xScale[entityID];
  float height = ecs->transformComponent->yScale[entityID];
  int vertArraySize =
      generateRectangleVertices(vertices, xPos, yPos, width, height);

  glGenVertexArrays(1, VAO);
  glGenBuffers(1, VBO);
  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBindVertexArray(*VAO);
  glBufferData(GL_ARRAY_BUFFER, vertArraySize * sizeof(float), &vertices,
               GL_STATIC_DRAW);
  // We tell OpenGL how to interpret the data per vertex; each vertex is 3
  // values
  // The first argument sets the location of the vertex attribute to 0 which is
  // reflected by our shader setting "layout (location = 0)"
  glVertexAttribPointer(0, COMPONENTS_PER_VERTEX, GL_FLOAT, GL_FALSE,
                        COMPONENTS_PER_VERTEX * sizeof(float), (void *)0);
  // We enable that array, giving the location as the argument
  glEnableVertexAttribArray(0);
  // Unbinds currently binded VBO/VAO
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
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
