#pragma once

#include "../../include/Global.h"
#include "../components/input_components.h"
#include "../components/physics_components.h"
#include "../components/render_components.h"
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

// Used to keep track of which entities have which components
// Increase size if num of components increases past 8
i32 initializeWindow(GLFWwindow **window, i32 windowWidth, i32 windowHeight);
void framebufferSizeCallback(GLFWwindow *window, i32 drawableWidth,
                             i32 drawableHeight);
i32 start(ECS *ecs, ComponentMask *entityComponentMasks);
void update(ECS *ecs, GLFWwindow *window, ComponentMask *entityComponentMasks);
void render(ECS *ecs);
i32 initializeAperture(GLFWwindow **window, ECS **ecs,
                       ComponentMask **entityComponentMasks, i32 windowWidth,
                       i32 windowHeight);
