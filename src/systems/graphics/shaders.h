#pragma once
#include "../../../include/Global.h"
#include </home/kasyap/Documents/Low_Level_Practice/ApertureGameEngine/include/glad/glad.h>

char *loadShaderSource(char *filename);
void setUniformVector4f(unsigned int shaderProgramID, char *uniformName,
                        f32 float1, f32 float2, f32 float3, f32 float4);
unsigned int compileAndLinkShaders(char *vertexShaderPath,
                                   char *fragmentShaderPath);
