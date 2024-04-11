#pragma once
#include "../../../include/Global.h"

#define COMPONENTS_PER_TEXTURED_VERTEX 5
#define COMPONENTS_PER_COLORED_VERTEX 3
#define UNIQUE_VERTICES_PER_RECTANGULAR_PRISM 8

static const u32 rec_prism_indices[] = {
    // Front face
    0, 1, 2, 2, 3, 0,
    // Right face
    0, 3, 7, 7, 4, 0,
    // Back face
    4, 7, 6, 6, 5, 4,
    // Left face
    5, 6, 2, 2, 1, 5,
    // Top face
    1, 0, 4, 4, 5, 1,
    // Bottom face
    3, 2, 6, 6, 7, 3};

u32 create_colored_rec_prism_vertices(f32 **vertices, f32 xScale, f32 yScale,
                                      f32 zScale);
