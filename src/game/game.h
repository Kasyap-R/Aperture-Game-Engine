#pragma once
#include "../core/ECS.h"
#include "../core/Engine.h"
#include "../systems/graphics/renderer.h"
#include "../systems/input/input.h"
#include "../systems/physics/physics.h"

#define PLAYER_ENTITY_ID 0
#define BALL_ENTITY_ID 1

void custom_start(ECS *ecs, ComponentMask *entityComponentMasks);
void custom_update(ECS *ecs, ComponentMask *entityComponentMasks,
                   EntityID entityID);
void custom_process_input(ECS *ecs, GLFWwindow *window);
void handle_ball_brick_collision(ECS *ecs, EntityID brickID, EntityID ballID);
