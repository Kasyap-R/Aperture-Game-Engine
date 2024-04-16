#include "camera.h"
#include <cglm/vec3.h>

Camera init_camera() {
  Camera camera = {.cameraPos = {50.0f, 50.0f, -50.0f},
                   .cameraFront = {0.0f, 0.0f, -1.0f},
                   .cameraUp = {0.0f, 1.0f, 0.0f}};
  return camera;
}
Camera update_player_camera(InputComponent *inputComponent, EntityID playerID,
                            Camera camera, f32 cameraSpeed) {
  vec3 movementDelta;
  glm_vec3_zero(movementDelta);
  cameraSpeed = cameraSpeed * SEC_PER_UPDATE;

  // Handle Forward and Backward Movement
  if (inputComponent->isWKeyPressed[playerID] == true) {
    glm_vec3_scale(camera.cameraFront, cameraSpeed, movementDelta);
  }
  if (inputComponent->isSKeyPressed[playerID] == true) {
    vec3 temp;
    glm_vec3_scale(camera.cameraFront, -cameraSpeed, temp);
    glm_vec3_add(movementDelta, temp, movementDelta);
  }

  // Handle right and left movement
  vec3 right;
  glm_vec3_cross(camera.cameraFront, camera.cameraUp, right);
  glm_vec3_scale(right, cameraSpeed, right);

  if (inputComponent->isDKeyPressed[playerID] == true) {
    glm_vec3_add(movementDelta, right, movementDelta);
  }

  vec3 left;
  glm_vec3_cross(camera.cameraFront, camera.cameraUp, left);
  glm_vec3_scale(left, cameraSpeed, left);

  if (inputComponent->isAKeyPressed[playerID] == true) {
    glm_vec3_add(movementDelta, left, movementDelta);
  }

  glm_vec3_add(camera.cameraPos, movementDelta, camera.cameraPos);

  return camera;
}
