#include "ECS.h"

bool hasComponent(uint8_t entityID, ComponentType type,
                  ComponentMask *entityComponentMasks) {
  return entityComponentMasks[entityID] & (1 << type);
}

void addComponentToEntity(uint8_t entityID, ComponentType type,
                          ComponentMask *entityComponentMasks) {
  // type represents which bit a component is represented by
  // we create a mask here where it is just 1 bit shifted to the types byte and
  // we OR to guarantee a 1 is there, which means that entity has that specific
  // component
  entityComponentMasks[entityID] |= (1 << type);
}

void removeComponentFromEntity(uint8_t entityID, ComponentType type,
                               ComponentMask *entityComponentMasks) {
  // Here we move a 1 to the position of the type then turn that into a zero and
  // everything else into a one. We then AND everything with 1 to return the
  // component mask to its original form, but now with the specified type marked
  // as zero
  entityComponentMasks[entityID] &= ~(1 << type);
}

void initECS(ECS *ecs) {
  memset(ecs->entityActive, 0, sizeof(ecs->entityActive));
  initTransformComponent(ecs->transformComponent);
  initVelocityComponent(ecs->velocityComponent);
  initInputComponent(ecs->inputComponent);
  initMeshComponent(ecs->meshComponent);
}
