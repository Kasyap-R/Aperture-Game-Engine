#include "input_components.h"
#include <string.h>

void initInputComponent(InputComponent *inputComponent) {
  memset(inputComponent->isWKeyPressed, 0,
         sizeof(inputComponent->isWKeyPressed));
  memset(inputComponent->isSKeyPressed, 0,
         sizeof(inputComponent->isSKeyPressed));
  memset(inputComponent->isAKeyPressed, 0,
         sizeof(inputComponent->isAKeyPressed));
  memset(inputComponent->isDKeyPressed, 0,
         sizeof(inputComponent->isAKeyPressed));
}

void setEntityInput(InputComponent *inputComponent, EntityID entityID,
                    bool isWKeyPressed, bool isSKeyPressed, bool isAPressed,
                    bool isDPressed) {
  inputComponent->isWKeyPressed[entityID] = isAPressed;
  inputComponent->isSKeyPressed[entityID] = isDPressed;
  inputComponent->isAKeyPressed[entityID] = isAPressed;
  inputComponent->isDKeyPressed[entityID] = isDPressed;
}
