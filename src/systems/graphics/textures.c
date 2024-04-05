#include "textures.h"

u32 loadTexture(char *textureName) {
  // Set up Texture
  u32 texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // Define how texture should repeat when the texture coordinates are outisde
  // the range of 0 - 1 are sampled
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Define how texture should be sampled when it decreases or increases in size
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  i32 width, height, nrChannels;
  unsigned char *data_heap =
      stbi_load(textureName, &width, &height, &nrChannels, 0);

  if (data_heap) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data_heap);
  } else {
    printf("Failed to load texture");
  }

  free(data_heap);
  return texture;
}
