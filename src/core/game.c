#include "game.h"
#include <GLFW/glfw3.h>

void custom_start(ECS *ecs, ComponentMask *entityComponentMasks) {
  // Set up player components
  PhysicsAttributes pAttributesPlayer = {-1000.0, -250.0, 600.0, 100.0,
                                         0.0,     0.0,    0.0};
  PhysicsAttributes pAttributesCircle = {0.0,  150.0, 90.0, 90.0,
                                         0.00, 50.0,  -90.0};

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
  f32 brickWidth = 2000 / (float)bricksPerLayer;
  f32 brickHeight = brickWidth / 6.0f;
  f32 yForCurrLayer = 562.5 + brickHeight / 2.0f;
  f32 initialX = -1000 - brickWidth / 2.0f;
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
  bool isAKeyPressed = ecs->inputComponent->isAKeyPressed[PLAYER_ENTITY_ID];
  bool isDKeyPressed = ecs->inputComponent->isDKeyPressed[PLAYER_ENTITY_ID];
  f32 playerVelocity = 100.0f;
  if (isAKeyPressed && isDKeyPressed) {
    physics_update_velocity(ecs->velocityComponent, PLAYER_ENTITY_ID, 0.0f,
                            0.0f, 0.0f);
  } else if (isAKeyPressed) {
    physics_update_velocity(ecs->velocityComponent, PLAYER_ENTITY_ID,
                            -playerVelocity, 0.0f, 0.0f);
  } else if (isDKeyPressed) {
    physics_update_velocity(ecs->velocityComponent, PLAYER_ENTITY_ID,
                            playerVelocity, 0.0f, 0.0f);
  } else {
    physics_update_velocity(ecs->velocityComponent, PLAYER_ENTITY_ID, 0.0f,
                            0.0f, 0.0f);
  }
}

void custom_update(ECS *ecs, ComponentMask *entityComponentMasks,
                   EntityID entityID) {
  if (physics_check_rectangle_circle_collision(ecs->transformComponent,
                                               entityID, BALL_ENTITY_ID)) {
    handle_ball_brick_collision(ecs, entityID, BALL_ENTITY_ID);
    if ((entityID != PLAYER_ENTITY_ID && entityID != BALL_ENTITY_ID)) {
      ecs->entityActive[entityID] = false;
    }
  }
}

void handle_ball_brick_collision(ECS *ecs, EntityID brickID, EntityID ballID) {
  static i32 lastCollidedBrick = -1;
  if (ballID == brickID || brickID == lastCollidedBrick) {
    return;
  }
  TransformComponent *transformComponents = ecs->transformComponent;
  f32 xBrick, widthBrick, xCircle, xMax, xMin, xDiff, xDiffNorm, maxXVelocity,
      newXVelocity, ballYVelocity;

  xBrick = transformComponents->x[brickID];
  widthBrick = transformComponents->xScale[brickID];
  xCircle = transformComponents->x[ballID];

  xMax = widthBrick / 2.0f + widthBrick * 0.1f;
  xMin = -widthBrick / 2.0f - widthBrick * 0.1f;
  xDiff = xCircle - xBrick;
  xDiffNorm = 2.0f * ((xDiff - xMin) / (xMax - xMin)) - 1.0f;

  maxXVelocity = 120.0;
  newXVelocity = xDiffNorm * maxXVelocity;

  ballYVelocity = ecs->velocityComponent->vY[ballID];
  physics_update_velocity(ecs->velocityComponent, ballID, newXVelocity,
                          -ballYVelocity, 0.0f);
  lastCollidedBrick = brickID;
}
