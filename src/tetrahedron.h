#pragma once

#include <stdint.h>
#include "mesh.h"

const Vector3 TETRAHEDRON_VERTICIES[] = {
    Vector3( 1.0f,  1.0f,  1.0f),
    Vector3(-1.0f, -1.0f,  1.0f),
    Vector3(-1.0f,  1.0f, -1.0f),
    Vector3( 1.0f, -1.0f, -1.0f)
};

const uint8_t TETRAHEDRON_EDGES[] = {
    0, 1,
    0, 2,
    0, 3,
    1, 2,
    1, 3,
    2, 3
};

Mesh const g_Tetrahedron(ARRAY_SIZE(TETRAHEDRON_VERTICIES), TETRAHEDRON_VERTICIES, ARRAY_SIZE(TETRAHEDRON_EDGES), TETRAHEDRON_EDGES);