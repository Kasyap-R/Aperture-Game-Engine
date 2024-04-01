#include "textures.h"

unsigned int loadTexture(ECS *ecs, char *textureName) {
  // Set up Texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // Define how texture should repeat when the texture coordinates are outisde
  // the range of 0 - 1 are sampled
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Define how texture should be sampled when it decreases or increases in size
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nrChannels;
  unsigned char *data_heap =
      stbi_load(textureName, &width, &height, &nrChannels, 0);

  printf("Width: %d\nHeight: %d\n", width, height);
  if (data_heap) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data_heap);
  } else {
    printf("Failed to load texture");
  }

  free(data_heap);
  return texture;
}
