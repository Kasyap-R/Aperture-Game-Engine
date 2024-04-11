#include "game.h"
#include <GLFW/glfw3.h>

#define CUBE_ID 0

void custom_start(ECS *ecs, ComponentMask *entityComponentMasks) {
  PhysicsAttributes pAttribs = {
      0.0, 0.0, 0.0, 300.0, 300.0, 300.0, 0.0, 0.0, 0.0,
  };
  physics_InstantiateEntity(ecs, CUBE_ID, pAttribs, entityComponentMasks);

  render_init_rec_prism(ecs, CUBE_ID, SHADER_COLORED, entityComponentMasks);
  render_AddColorComponent(ecs, CUBE_ID, entityComponentMasks, 1.0f, 0.0f, 0.0f,
                           1.0f);
}

void custom_process_input(ECS *ecs, GLFWwindow *window) {}

void custom_update(ECS *ecs, ComponentMask *entityComponentMasks,
                   EntityID entityID) {}
