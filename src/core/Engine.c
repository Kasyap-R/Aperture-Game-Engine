#include "Engine.h"
#include "ECS.h"

#define PLAYER_ENTITY_ID 0
#define BALL_ENTITY_ID 1

int main(void) {
  GLFWwindow *window;
  ECS *ecs;
  ComponentMask *entityComponentMasks;

  i32 windowWidth = 1920;
  i32 windowHeight = 1080;

  initializeAperture(&window, &ecs, &entityComponentMasks, windowWidth,
                     windowHeight);

  f64 prevTime = glfwGetTime() * 1000;
  f64 lag = 0.0;
  f64 timeSinceLastSecond = 0.0;
  i32 FPS = 0;

  start(ecs, entityComponentMasks);

  // main game loop
  while (!glfwWindowShouldClose(window)) {
    f64 currentTime = glfwGetTime() * 1000;
    f64 deltaTime = currentTime - prevTime;
    timeSinceLastSecond += deltaTime;
    prevTime = currentTime;
    lag += deltaTime;
    FPS += 1;

    if ((timeSinceLastSecond) >= 1000) {
      printf("FPS: %d\n", FPS);
      timeSinceLastSecond = 0;
      FPS = 0;
      fflush(stdout);
    }

    update(ecs, window, entityComponentMasks);

    // Ensure game time catches up with real time
    while (lag >= MS_PER_UPDATE) {
      glfwPollEvents();
      update(ecs, window, entityComponentMasks);
      lag -= MS_PER_UPDATE;
    }

    // Clear back buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Turquoise color
    glClear(GL_COLOR_BUFFER_BIT);
    // Render new data to back buffer
    // Swap Buffers
    render(ecs);

    glfwSwapBuffers(window);
  }

  free(ecs);
  // Deletes all of GLFW's allocated resources
  glfwTerminate();
  return 0;
}

// Return value is success code
i32 start(ECS *ecs, ComponentMask *entityComponentMasks) {
  // Set up player components
  PhysicsAttributes pAttributesPlayer = {0.0, -0.25, 0.3, 0.1, 0.0, 0.0, 0.0};
  PhysicsAttributes pAttributesCircle = {0.0, 0.25, 0.3, 0.3, 0.0, 0.01, -0.01};

  // Player Setup
  input_InstantiateEntity(ecs, PLAYER_ENTITY_ID, entityComponentMasks);
  physics_InstantiateEntity(ecs, PLAYER_ENTITY_ID, pAttributesPlayer,
                            entityComponentMasks);
  render_InstantiateRectangleEntity(ecs, PLAYER_ENTITY_ID, SHADER_TEXTURED,
                                    entityComponentMasks);
  render_AddSpriteComponent(ecs, PLAYER_ENTITY_ID, entityComponentMasks,
                            "textures/platform_texture.png");

  // Ball Setup
  physics_InstantiateEntity(ecs, BALL_ENTITY_ID, pAttributesCircle,
                            entityComponentMasks);
  render_InstantiateCircleEntity(ecs, BALL_ENTITY_ID, SHADER_TEXTURED,
                                 entityComponentMasks);
  render_AddSpriteComponent(ecs, BALL_ENTITY_ID, entityComponentMasks,
                            "textures/platform_texture.png");
  return 0;
}

void update(ECS *ecs, GLFWwindow *window, ComponentMask *entityComponentMasks) {
  for (EntityID entityID = 0; entityID < MAX_ENTITIES; entityID++) {
    if (!ecs->entityActive[entityID]) {
      continue;
    }
    if (hasComponent(entityID, COMPONENT_INPUT, entityComponentMasks)) {
      input_ProcessInput(ecs, window, entityID);
      physics_ProcessInput(ecs, entityID);
    }
    physics_UpdateEntityPosition(ecs, entityID);
  }
}

void render(ECS *ecs) {
  render_RenderEntity(ecs, PLAYER_ENTITY_ID);
  render_RenderEntity(ecs, BALL_ENTITY_ID);
}

i32 initializeAperture(GLFWwindow **window, ECS **ecs,
                       ComponentMask **entityComponentMasks, i32 windowWidth,
                       i32 windowHeight) {
  if (initializeWindow(window, windowWidth, windowHeight) != 0) {
    return -1;
  }

  i32 drawableWidth;
  i32 drawableHeight;

  glfwGetFramebufferSize(*window, &drawableWidth, &drawableHeight);
  glViewport(0, 0, drawableWidth, drawableHeight);

  *ecs = malloc(sizeof(ECS));
  i32 maskSize = sizeof(ComponentMask) * MAX_ENTITIES;
  *entityComponentMasks = malloc(maskSize);

  initECS(*ecs);
  initEntityComponentMasks(*entityComponentMasks, maskSize);

  render_LoadShaders();
  return 0;
}

void framebufferSizeCallback(GLFWwindow *window, i32 drawableWidth,
                             i32 drawableHeight) {
  glViewport(0, 0, drawableWidth, drawableHeight);
}

// f64 pointer is used so that initWindow can change which window the window
// pointer points to
i32 initializeWindow(GLFWwindow **window, i32 windowWidth, i32 windowHeight) {
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
