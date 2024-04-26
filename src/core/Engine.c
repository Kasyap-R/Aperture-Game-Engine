#include "Engine.h"
#include "../game/game.h"
#include "ECS.h"
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <sys/types.h>

#define PLAYER_ENTITY_ID 0
#define BALL_ENTITY_ID 1

int main(void) {
  GLFWwindow *window;
  ECS *ecs;
  ComponentMask *entityComponentMasks;

  init_aperture(&window, &ecs, &entityComponentMasks);

  f64 prevTime = glfwGetTime() * 1000;
  f64 lag = 0.0;

  start(ecs, entityComponentMasks);
  custom_start(ecs, entityComponentMasks);

  // main game loop
  while (!glfwWindowShouldClose(window)) {
    f64 currentTime = glfwGetTime() * 1000;
    f64 deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    lag += deltaTime;

    calculate_fps(deltaTime);

    // Ensure game time catches up with real time
    while (lag >= MS_PER_UPDATE) {
      glfwPollEvents();
      update(ecs, window, entityComponentMasks);
      lag -= MS_PER_UPDATE;
    }

    // Clear back buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Turquoise color
    glClear(GL_COLOR_BUFFER_BIT);

    render(ecs);
    glfwSwapBuffers(window);
  }

  free_resources(ecs, entityComponentMasks);

  return 0;
}

// Return value is success code
i32 start(ECS *ecs, ComponentMask *entityComponentMasks) {
  render_LoadShaders();
  render_init_projection_matrix();
  return 0;
}

void update(ECS *ecs, GLFWwindow *window, ComponentMask *entityComponentMasks) {
  input_check_window_close(window);
  custom_process_input(ecs, window);
  for (EntityID entityID = 0; entityID < MAX_ENTITIES; entityID++) {
    if (!ecs->entityActive[entityID]) {
      continue;
    }
    custom_update(ecs, entityComponentMasks, entityID);
    physics_UpdateEntityPosition(ecs, entityID);
  }
}

void render(ECS *ecs) {
  render_calculate_view_matrix(ecs, CAMERA_ID);
  for (EntityID entityID = 0; entityID < MAX_ENTITIES; entityID++) {
    if (!ecs->entityActive[entityID]) {
      continue;
    }
    if (entityID == CAMERA_ID) {
      continue;
    }
    printf("RENDERING: %d\n", entityID);
    render_RenderEntity(ecs, entityID);
  }
}

i32 init_aperture(GLFWwindow **window, ECS **ecs,
                  ComponentMask **entityComponentMasks) {
  init_glfw();

  i32 windowWidth = 1920;
  i32 windowHeight = 1080;
  if (init_window(window, windowWidth, windowHeight) != 0) {
    exit(EXIT_FAILURE);
  }

  load_opengl_functions();

  i32 drawableWidth;
  i32 drawableHeight;
  glfwGetFramebufferSize(*window, &drawableWidth, &drawableHeight);
  glViewport(0, 0, drawableWidth, drawableHeight);

  *ecs = malloc(sizeof(ECS));
  i32 maskSize = sizeof(ComponentMask) * MAX_ENTITIES;
  *entityComponentMasks = malloc(maskSize);

  initECS(*ecs);
  initEntityComponentMasks(*entityComponentMasks, maskSize);

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, i32 drawableWidth,
                               i32 drawableHeight) {
  glViewport(0, 0, drawableWidth, drawableHeight);
}

i32 init_window(GLFWwindow **window, i32 windowWidth, i32 windowHeight) {

  *window = glfwCreateWindow(windowWidth, windowHeight,
                             "Aperture Engine Window", NULL, NULL);

  if (!*window) {
    glfwTerminate();
    printf("Failed to create GLFW window\n");
    return -1;
  }

  glfwSetFramebufferSizeCallback(*window, framebuffer_size_callback);

  glfwMakeContextCurrent(*window);

  return 0;
}

void free_resources(ECS *ecs, ComponentMask *entityComponentMasks) {
  free(ecs);
  free(entityComponentMasks);
  glfwTerminate();
}

void calculate_fps(f64 deltaTime) {
  static i32 FPS = 0;
  static f64 timeElapsedMilliseconds = 0;
  timeElapsedMilliseconds += deltaTime;
  FPS += 1;
  if (timeElapsedMilliseconds > 1000) {
    printf("FPS: %d\n", FPS);
    timeElapsedMilliseconds = 0.0;
    FPS = 0;
  }
}

void init_glfw() {
  if (!glfwInit()) {
    printf("Failed to initialize GLFW\n");
    exit(EXIT_FAILURE);
  }
}

void load_opengl_functions() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    exit(EXIT_FAILURE);
  }
}
