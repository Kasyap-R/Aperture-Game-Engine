#pragma once
#include "../../../include/Global.h"
#include "../../core/ECS.h"

void update_view_matrix(InputComponent *inputComponent,
                        VelocityComponent velocityComponent, EntityID cameraID);
