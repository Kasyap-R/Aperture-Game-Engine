#pragma once

#include </home/kasyap/Documents/Low_Level_Practice/ApertureGameEngine/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

int initializeWindow(GLFWwindow **window, int windowWidth, int windowHeight);
void framebufferSizeCallback(GLFWwindow *window, int drawableWidth,
                             int drawableHeight);
void processInput(GLFWwindow *window);
