#include "shaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUniformVector4f(u32 shaderProgramID, char *uniformName, f32 float1,
                        f32 float2, f32 float3, f32 float4) {
  int location = glGetUniformLocation(shaderProgramID, uniformName);
  glUniform4f(location, float1, float2, float3, float4);
}

u32 compileAndLinkShaders(char *vertexShaderPath, char *fragmentShaderPath) {
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
  GLuint shaderProgramID = glCreateProgram();
  glAttachShader(shaderProgramID, vertexShader);
  glAttachShader(shaderProgramID, fragmentShader);
  glLinkProgram(shaderProgramID);
  glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderProgramID, 512, NULL, infoLog);
    printf("ERROR::SHADER::VERTEX|FRAGMENT::LINKING_FAILED\n%s\n", infoLog);
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgramID;
}

char *loadShaderSource(char *filename) {
  FILE *file;
  char ch;

  file = fopen(filename, "r");

  if (NULL == file) {
    perror("Failed to Open Shader File");
    exit(1);
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    perror("Failed to find end of file");
    fclose(file);
    exit(1);
  }

  long fileSize = ftell(file);
  if (fileSize == -1) {
    perror("Failed to get file size");
    fclose(file);
    exit(1);
  }

  rewind(file);

  char *content_heap = malloc(fileSize + 1); // +1 for null terminator
  if (content_heap == NULL) {
    perror("Failed to allocate memory");
    fclose(file);
    exit(1);
  }

  // Read bytes into memory
  usize bytesRead = fread(content_heap, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    if (feof(file)) {
      printf("Unexpected end of file.\n");
    } else if (ferror(file)) {
      perror("Error reading file");
    }
    free(content_heap);
    fclose(file);
    exit(1);
  }

  content_heap[bytesRead] = '\0';

  fclose(file);

  return content_heap;
}
