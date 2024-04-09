#include "input.h"
#include <GLFW/glfw3.h>

void input_ProcessInput(ECS *ecs, GLFWwindow *window, EntityID entityID) {
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    ecs->inputComponent->isAKeyPressed[entityID] = true;
  } else {
    ecs->inputComponent->isAKeyPressed[entityID] = false;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    ecs->inputComponent->isDKeyPressed[entityID] = true;
  } else {
    ecs->inputComponent->isDKeyPressed[entityID] = false;
  }
}

void input_InstantiateEntity(ECS *ecs, EntityID entityID,
                             ComponentMask *entityComponentMasks) {
  addComponentToEntity(ecs, entityID, COMPONENT_INPUT, entityComponentMasks);
}

void input_check_window_close(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
