#pragma once
#include "../../../include/Global.h"
#include "../../components/input_components.h"
#include <cglm/cglm.h>

typedef struct {
  vec3 cameraPos;
  vec3 cameraFront;
  vec3 cameraUp;
} Camera;

Camera update_player_camera(InputComponent *inputComponent, EntityID plyaerID,
                            Camera camera, f32 cameraSpeed);
Camera init_camera();
