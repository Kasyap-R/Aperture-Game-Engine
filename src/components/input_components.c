#include "input_components.h"
#include <string.h>

void initInputComponent(InputComponent *inputComponent) {
  memset(inputComponent->isAKeyPressed, 0,
         sizeof(inputComponent->isAKeyPressed));
  memset(inputComponent->isDKeyPressed, 0,
         sizeof(inputComponent->isDKeyPressed));
  memset(inputComponent->isWKeyPressed, 0,
         sizeof(inputComponent->isWKeyPressed));
  memset(inputComponent->isSKeyPressed, 0,
         sizeof(inputComponent->isSKeyPressed));
}

void setEntityInput(InputComponent *inputComponent, EntityID entityID,
                    bool isWPressed, bool isSPressed, bool isAPressed,
                    bool isDPressed) {
  inputComponent->isAKeyPressed[entityID] = isAPressed;
  inputComponent->isSKeyPressed[entityID] = isSPressed;
  inputComponent->isWKeyPressed[entityID] = isWPressed;
  inputComponent->isDKeyPressed[entityID] = isDPressed;
}
