#pragma once

#include "../../components/input_components.h"
#include "../../core/ECS.h"
#include </home/kasyap/Documents/Low_Level_Practice/ApertureGameEngine/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>

void input_InstantiatePlayerEntity(ECS *ecs, uint8_t entityID,
                                   ComponentMask *entityComponentMasks);
void input_ProcessInput(ECS *ecs, GLFWwindow *window, uint8_t entityID);
