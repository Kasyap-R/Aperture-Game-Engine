#include "game.h"
#include <GLFW/glfw3.h>

void custom_start(ECS *ecs, ComponentMask *entityComponentMasks) {
  // Set up player components
  PhysicsAttributes pAttributesPlayer = {0.0, -0.25, 0.5, 0.1, 0.0, 0.0, 0.0};
  PhysicsAttributes pAttributesCircle = {0.0,  0.25,   0.09, 0.09,
                                         0.00, -0.003, -0.03};

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
  render_InstantiateCircleEntity(ecs, BALL_ENTITY_ID, SHADER_COLORED,
                                 entityComponentMasks);
  render_AddColorComponent(ecs, BALL_ENTITY_ID, entityComponentMasks, 1.0, 0.0f,
                           0.0f, 1.0f);

  // Brick Layer Setup
  i32 currEntityNum = 2;
  i32 numBricks = 20;
  i32 bricksPerLayer = 5;
  i32 numLayers = numBricks / bricksPerLayer;
  f32 brickWidth = 2.0f / (float)bricksPerLayer;
  f32 brickHeight = brickWidth / 3.0f;
  f32 yForCurrLayer = 1 + brickHeight / 2.0f;
  f32 initialX = -1 - brickWidth / 2.0f;
  for (u8 layer = 1; layer <= numLayers; layer++) {
    yForCurrLayer -= brickHeight;
    for (u8 brickNum = 1; brickNum <= bricksPerLayer; brickNum++) {
      PhysicsAttributes pAttribs = {
          initialX + brickWidth * brickNum,
          yForCurrLayer,
          brickWidth,
          brickHeight,
          0.0,
          0.0,
          0.0,
      };
      physics_InstantiateEntity(ecs, currEntityNum, pAttribs,
                                entityComponentMasks);
      render_InstantiateRectangleEntity(ecs, currEntityNum, SHADER_TEXTURED,
                                        entityComponentMasks);
      render_AddSpriteComponent(ecs, currEntityNum, entityComponentMasks,
                                "textures/brick_art.png");

      currEntityNum++;
    }
  }
}

void custom_process_input(ECS *ecs, GLFWwindow *window) {
  input_ProcessInput(ecs, window, PLAYER_ENTITY_ID);
}

void custom_update(ECS *ecs, ComponentMask *entityComponentMasks,
                   EntityID entityID) {
  if (entityID == PLAYER_ENTITY_ID) {
    physics_ProcessInput(ecs, entityID);
  }
  if (physics_CheckForCollision(ecs, entityID, BALL_ENTITY_ID) &&
      (entityID != PLAYER_ENTITY_ID)) {
    ecs->entityActive[entityID] = false;
  }
}
