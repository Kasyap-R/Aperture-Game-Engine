#include "shaders.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  size_t bytesRead = fread(content_heap, sizeof(char), fileSize, file);
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
