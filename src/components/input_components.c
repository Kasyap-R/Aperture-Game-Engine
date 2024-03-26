#include "input_components.h"

void initInputComponent(InputComponent *inputComponent) {
  inputComponent->isAKeyPressed = false;
  inputComponent->isDKeyPressed = false;
}

void setEntityInput(InputComponent *inputComponent, bool isAPressed, bool isDPressed) {
    inputComponent->isAKeyPressed = isAPressed;
    inputComponent->isDKeyPressed = isDPressed;
}