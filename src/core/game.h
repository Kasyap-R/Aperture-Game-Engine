#pragma once
#include "../systems/graphics/renderer.h"
#include "../systems/input/input.h"
#include "../systems/physics/physics.h"
#include "ECS.h"
#include "Engine.h"

#define PLAYER_ENTITY_ID 0
#define BALL_ENTITY_ID 1

void custom_start(ECS *ecs, ComponentMask *entityComponentMasks);
void custom_update(ECS *ecs, ComponentMask *entityComponentMasks,
                   EntityID entityID);
void custom_process_input(ECS *ecs, GLFWwindow *window);
