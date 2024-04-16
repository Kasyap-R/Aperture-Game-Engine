#include "game.h"
#include <GLFW/glfw3.h>

#define CUBE_ID 0
#define PLAYER_ID 1

void custom_start(ECS *ecs, ComponentMask *entityComponentMasks) {
  PhysicsAttributes pAttribs = {
      0.0, 0.0, -50.0, 50.0, 50.0, 50.0, 0.0, 0.0, 0.0,
  };
  physics_InstantiateEntity(ecs, CUBE_ID, pAttribs, entityComponentMasks);
  printf("MASK: %d\n", entityComponentMasks[0]);
  if (ecs->entityActive[0]) {
    printf("Its Active\n");
  }

  render_init_rec_prism(ecs, CUBE_ID, SHADER_TEXTURED, entityComponentMasks);
  render_AddSpriteComponent(ecs, CUBE_ID, entityComponentMasks,
                            "textures/brick_art.png");
}

void custom_process_input(ECS *ecs, GLFWwindow *window) {}

void custom_update(ECS *ecs, ComponentMask *entityComponentMasks,
                   EntityID entityID) {
  // Note, have to create camera component later
  /* if (entityID == PLAYER_ID) {
    printf("Updating Camera\n");
    render_update_camera(ecs->inputComponent, PLAYER_ID);
  } */
}
