#include "Engine.h"
#include "ECS.h"

#define PLAYER_ENTITY_ID 0

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

  // Setting the viewport to cover the whole window
  glViewport(0, 0, drawableWidth, drawableHeight);

  ECS ecs;
  initECS(&ecs);
  ComponentMask entityComponentMasks[MAX_ENTITIES];

  double prevTime = glfwGetTime() * 1000;
  double lag = 0.0;

  // TODO: Add Material Component so we don't have to worry about passing this
  // value back on forth
  uint shaderProgram;
  shaderProgram = start(&ecs, entityComponentMasks);

  // main game loop
  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime() * 1000;
    double deltaTime = currentTime - prevTime;
    prevTime = currentTime;
    lag += deltaTime;

    // Ensure game time catches up with real time
    while (lag >= MS_PER_UPDATE) {
      glfwPollEvents();
      update(&ecs, window, entityComponentMasks);
      lag -= MS_PER_UPDATE;
    }

    // Clear back buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Turquoise color
    glClear(GL_COLOR_BUFFER_BIT);
    // Render new data to back buffer
    // Swap Buffers
    render(&ecs, shaderProgram);

    glfwSwapBuffers(window);
  }

  // Deletes all of GLFW's allocated resources
  glfwTerminate();
  return 0;
}

int start(ECS *ecs, ComponentMask *entityComponentMasks) {
  // Set up player components
  // TODO: Find an elegant solution to setting entities as active
  // Maybe do a check whenever you call addComponent()?
  ecs->entityActive[PLAYER_ENTITY_ID] = true;
  input_InstantiatePlayerEntity(ecs, PLAYER_ENTITY_ID, entityComponentMasks);
  physics_InstantiatePlayerEntity(ecs, PLAYER_ENTITY_ID, entityComponentMasks);
  return render_InstantiatePlayerEntity(ecs, PLAYER_ENTITY_ID,
                                        entityComponentMasks);
}

void update(ECS *ecs, GLFWwindow *window, ComponentMask *entityComponentMasks) {
  for (uint8_t entityID = 0; entityID < MAX_ENTITIES; entityID++) {
    if (!ecs->entityActive[entityID]) {
      continue;
    }
    if (hasComponent(entityID, COMPONENT_INPUT, entityComponentMasks)) {
      input_ProcessInput(ecs, window, PLAYER_ENTITY_ID);
      physics_UpdatePlayerEntity(ecs, PLAYER_ENTITY_ID);
    }
  }
}

void render(ECS *ecs, uint shaderProgram) {
  render_RenderComponent(ecs, PLAYER_ENTITY_ID, shaderProgram);
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
