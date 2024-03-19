#include "Engine.h"
#include <stdbool.h>

int main(void) {
  GLFWwindow *window;

  int windowWidth = 1920;
  int windowHeight = 1080;

  if (initializeWindow(&window, windowWidth, windowHeight) != 0) {
    return -1;
  }

  int drawableWidth;
  int drawableHeight;

  // Getting the size of the drawable area in the window
  glfwGetFramebufferSize(window, &drawableWidth, &drawableHeight);
  printf("Viewport Width: %d\nViewport Height: %d", drawableWidth,
         drawableHeight);

  // Setting the viewport to cover the whole window
  glViewport(0, 0, drawableWidth, drawableHeight);

  // main game loop
  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    glfwPollEvents();                     // poll for events and process them
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Turquoise color
    glClear(GL_COLOR_BUFFER_BIT); // clear back buffer with specified color
    // Rendering test
    glBegin(GL_TRIANGLES);
    glColor3f(1.0f, 0.0f, 0.0f); // Red color
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.0f, 0.5f);
    glEnd();
    glfwSwapBuffers(window); // swap front and back buffers
  }

  // Deletes all of GLFW's allocated resources
  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebufferSizeCallback(GLFWwindow *window, int drawableWidth,
                             int drawableHeight) {
  glViewport(0, 0, drawableWidth, drawableHeight);
}

// Double pointer is used so that initWindow can change which window the window
// pointer points to
int initializeWindow(GLFWwindow **window, int windowWidth, int windowHeight) {
  if (!glfwInit()) {
    printf("Failed to initialize GLFW");
    return -1;
  }

  /* Disables Window Decorations (i.e. borders, name, etc.);
   * glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
   */

  // When we create a window, a context is created which is basically used to
  // store state for the window. The values of the pixels themselves are stored
  // in buffers
  // NOTE: We can pass glfwGetPrimaryMonitor() instead of the first NULL to make
  // it fullscreen mode
  *window = glfwCreateWindow(windowWidth, windowHeight,
                             "Aperture Engine Window", NULL, NULL);

  if (!*window) {
    glfwTerminate();
    printf("Failed to create GLFW window\n");
    return -1;
  }

  // Set a callback so the viewport can dynamically resize when the user resizes
  // the window
  glfwSetFramebufferSizeCallback(*window, framebufferSizeCallback);

  // Tells GLFW that subsequent OpenGL calls should affect this window's context
  // The contex is basically all the state neccesary for rendering to a window
  // (buffers, other state info, etc.)
  glfwMakeContextCurrent(*window);

  // Calls OpenGL function pointers
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    return -1;
  }

  return 0;
}
