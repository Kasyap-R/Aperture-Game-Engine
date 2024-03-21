#pragma once

#include "../../include/Global.h"
#include "../components/input/input.h"
#include "../components/physics/components.h"
#include "../systems/graphics/renderer.h"
#include "../systems/input/input.h"
#include "../systems/physics/physics.h"
#include "ECS.h"
#include </home/kasyap/Documents/Low_Level_Practice/ApertureGameEngine/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define PLAYER_ENTITY_ID 0

// Used to keep track of which entities have which components
// Increase size if num of components increases past 8
int initializeWindow(GLFWwindow **window, int windowWidth, int windowHeight);
void framebufferSizeCallback(GLFWwindow *window, int drawableWidth,
                             int drawableHeight);
void processInput(GLFWwindow *window);
void update();
void render(ECS *ecs);
