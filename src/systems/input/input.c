#include "input.h"
#include <GLFW/glfw3.h>

void input_ProcessInput(ECS *ecs, GLFWwindow *window, uint8_t entityID) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
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

void input_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                   ComponentMask *entityComponentMasks) {
  addComponentToEntity(entityID, COMPONENT_INPUT, entityComponentMasks);
}
