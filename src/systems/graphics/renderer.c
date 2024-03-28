#include "renderer.h"
#include "shaders.h"
#include <stdlib.h>
#include <string.h>

#define COMPONENTS_PER_VERTEX 3
#define VERTICES_PER_RECTANGE 6

void render_RenderComponent(ECS *ecs, uint8_t entityID, uint shaderProgram) {
  glUseProgram(shaderProgram);
  unsigned int VAO = ecs->meshComponent->VAO[entityID];
  unsigned int numVerts = ecs->meshComponent->vertexCount[entityID];
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, numVerts);
  glBindVertexArray(0);
}

uint render_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                    ComponentMask *entityComponentMasks) {
  // I assume the size of the vertices array here, must be fixed later
  float vertices[VERTICES_PER_RECTANGE * COMPONENTS_PER_VERTEX];
  float xPos = ecs->transformComponent->x[entityID];
  float yPos = ecs->transformComponent->y[entityID];
  float width = ecs->transformComponent->xScale[entityID];
  float height = ecs->transformComponent->yScale[entityID];

  int vertArraySize =
      generateRectangleVertices(vertices, xPos, yPos, width, height);

  unsigned int VBO, VAO;
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
  addComponentToEntity(entityID, COMPONENT_MESH, entityComponentMasks);
  setEntityMesh(ecs->meshComponent, entityID, VAO, VBO, VERTICES_PER_RECTANGE);
  // TODO: get rid of this and make a material component instead of returning
  // shaderProgram
  return compileAndLinkShaders("shaders/player.vert", "shaders/player.frag");
}

unsigned int compileAndLinkShaders(char *vertexShaderPath,
                                   char *fragmentShaderPath) {
  // Compiling Shader
  char *vertexShaderSource_heap = loadShaderSource(vertexShaderPath);
  char *fragmentShaderSource_heap = loadShaderSource(fragmentShaderPath);
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

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  source = fragmentShaderSource_heap;
  glShaderSource(fragmentShader, 1, &source, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
  }

  free(vertexShaderSource_heap);
  free(fragmentShaderSource_heap);

  // Linking Shader Program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX|FRAGMENT::LINKING_FAILED\n%s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // TODO: In future, have a material component
  return shaderProgram;
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
