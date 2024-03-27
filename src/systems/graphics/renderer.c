#include "renderer.h"
#include "shaders.h"

#include <stdlib.h>
#include <string.h>

#define COMPONENTS_PER_VERTEX 3
#define VERTICES_PER_RECTANGE 6

void render_RenderComponent(ECS *ecs, uint8_t entityID) {
  // Compiling Shader
  char *vertexShaderSource_heap = loadShaderSource("shaders/player.vert");
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar *source = vertexShaderSource_heap;
  glShaderSource(vertexShader, 1, &source, NULL);
  glCompileShader(vertexShader);

  GLint success;
  GLchar infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
  }

  free(vertexShaderSource_heap);

  // Linking Shader Program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  // Attach fragment shader here when neccesary
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX::LINKING_FAILED\n%s\n", infoLog);
  }

  glDeleteShader(vertexShader);

  glUseProgram(shaderProgram);
  unsigned int VAO = ecs->meshComponent->VAO[entityID];
  unsigned int VBO = ecs->meshComponent->VBO[entityID];
  unsigned int numVerts = ecs->meshComponent->vertexCount[entityID];
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, numVerts);
  glBindVertexArray(0);
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
