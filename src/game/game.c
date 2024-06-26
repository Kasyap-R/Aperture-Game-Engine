#include "game.h"
#include <GLFW/glfw3.h>

#define CUBE_ID 1

void custom_start(ECS *ecs, ComponentMask *entityComponentMasks) {
  // Set up Camera/Player
  PhysicsAttributes playerPhysAttribs = {
      -400.0f, 90.0f, 90.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
  };
  input_InstantiateEntity(ecs, CAMERA_ID, entityComponentMasks);
  physics_InstantiateEntity(ecs, CAMERA_ID, playerPhysAttribs,
                            entityComponentMasks);

  // Set up Cube
  PhysicsAttributes pAttribs = {
      0.0, 0.0, 0.0, 100.0, 100.0, 100.0, 0.0, 0.0, 0.0,
  };
  physics_InstantiateEntity(ecs, CUBE_ID, pAttribs, entityComponentMasks);

  render_init_rec_prism(ecs, CUBE_ID, SHADER_COLORED, entityComponentMasks);
  render_AddColorComponent(ecs, CUBE_ID, entityComponentMasks, 1.0f, 0.0f, 0.0f,
                           1.0f);
}

void custom_process_input(ECS *ecs, GLFWwindow *window) {
  input_ProcessInput(ecs, window, CAMERA_ID);
}

void custom_update(ECS *ecs, ComponentMask *entityComponentMasks,
                   EntityID entityID) {}
