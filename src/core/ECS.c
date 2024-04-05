#include "ECS.h"

bool hasComponent(EntityID entityID, ComponentType type,
                  ComponentMask *entityComponentMasks) {
  return entityComponentMasks[entityID] & (1 << type);
}

void addComponentToEntity(ECS *ecs, EntityID entityID, ComponentType type,
                          ComponentMask *entityComponentMasks) {
  ecs->entityActive[entityID] = true;
  // type represents which bit a component is represented by
  // we create a mask here where it is just 1 bit shifted to the types byte and
  // we OR to guarantee a 1 is there, which means that entity has that specific
  // component
  entityComponentMasks[entityID] |= (1 << type);
}

void removeComponentFromEntity(ECS *ecs, EntityID entityID, ComponentType type,
                               ComponentMask *entityComponentMasks) {
  // Here we move a 1 to the position of the type then turn that into a zero and
  // everything else into a one. We then AND everything with 1 to return the
  // component mask to its original form, but now with the specified type marked
  // as zero
  entityComponentMasks[entityID] &= ~(1 << type);
  if (entityComponentMasks[entityID] == 0) {
    ecs->entityActive[entityID] = false;
  }
}

void initECS(ECS *ecs) {
  memset(ecs->entityActive, 0, sizeof(ecs->entityActive));
  initTransformComponent(ecs->transformComponent);
  initVelocityComponent(ecs->velocityComponent);
  initInputComponent(ecs->inputComponent);
  initMeshComponent(ecs->meshComponent);
  initMaterialComponent(ecs->materialComponent);
  initColorComponent(ecs->colorComponent);
  initSpriteComponent(ecs->spriteComponent);
}

void initEntityComponentMasks(ComponentMask *entityComponentMasks,
                              usize maskSize) {
  memset(entityComponentMasks, 0, maskSize);
}
