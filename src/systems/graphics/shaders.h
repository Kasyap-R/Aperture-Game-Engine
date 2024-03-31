#pragma once
#include </home/kasyap/Documents/Low_Level_Practice/ApertureGameEngine/include/glad/glad.h>

char *loadShaderSource(char *filename);
void setUniform4f(unsigned int shaderProgramID, char *uniformName, float float1,
                  float float2, float float3, float float4);
