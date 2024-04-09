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

i32 init_window(GLFWwindow **window, i32 windowWidth, i32 windowHeight);
void framebuffer_size_callback(GLFWwindow *window, i32 drawableWidth,
                               i32 drawableHeight);
i32 start(ECS *ecs, ComponentMask *entityComponentMasks);
void update(ECS *ecs, GLFWwindow *window, ComponentMask *entityComponentMasks);
void render(ECS *ecs);
i32 init_aperture(GLFWwindow **window, ECS **ecs,
                  ComponentMask **entityComponentMasks);
void run();
void calculate_fps(f64 deltaTime);
void free_resources(ECS *ecs, ComponentMask *entityComponentMasks);
void init_glfw();
void load_opengl_functions();
