#include "Engine.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

// Double pointer is used so that initWindow can change which window the window
// pointer points to
int initializeWindow(GLFWwindow **window, int width, int height) {
  if (!glfwInit()) {
    printf("Failed to initialize GLFW");
    return -1;
  }

  // When we create a window, a context is created which is basically used to
  // store state for the window. The values of the pixels themselves are stored
  // in buffers
  *window =
      glfwCreateWindow(width, height, "Aperture Engine Window", NULL, NULL);

  if (!*window) {
    glfwTerminate();
    printf("Failed to create GLFW window\n");
    return -1;
  }

  // Tells GLFW that subsequent OpenGL calls should affect this window's context
  glfwMakeContextCurrent(*window);

  return 0;
}

int main(void) {
  GLFWwindow *window;

  if (initializeWindow(&window, 1920, 1080) != 0) {
    return -1;
  }

  // main game loop
  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT); // render new data to back buffer
    glfwSwapBuffers(window);      // swap front and back buffers
    glfwPollEvents();             // poll for events and process them
  }

  glfwTerminate();
  return 0;
}
