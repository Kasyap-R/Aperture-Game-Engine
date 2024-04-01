#pragma once

#include "../../../lib/stb_image.h"
#include "../../../src/core/ECS.h"
#include "../../../src/core/Engine.h"

// In the future, this should be passed some sort of texture/sprite component
unsigned int loadTexture(char *textureName);
