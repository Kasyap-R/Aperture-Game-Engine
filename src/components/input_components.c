#include "input_components.h"
#include <string.h>

void initInputComponent(InputComponent *inputComponent) {
  memset(inputComponent->isAKeyPressed, 0,
         sizeof(inputComponent->isAKeyPressed));
  memset(inputComponent->isDKeyPressed, 0,
         sizeof(inputComponent->isAKeyPressed));
}

void setEntityInput(InputComponent *inputComponent, uint8_t entityID,
                    bool isAPressed, bool isDPressed) {
  inputComponent->isAKeyPressed[entityID] = isAPressed;
  inputComponent->isDKeyPressed[entityID] = isDPressed;
}
