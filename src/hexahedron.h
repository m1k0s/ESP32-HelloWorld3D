#pragma once

#include <stdint.h>
#include "mesh.h"

const Vector3 HEXAHEDRON_VERTICIES[] = {
    Vector3(-1.0f, -1.0f, -1.0f),
    Vector3(-1.0f,  1.0f, -1.0f),
    Vector3( 1.0f,  1.0f, -1.0f),
    Vector3( 1.0f, -1.0f, -1.0f),
    Vector3(-1.0f, -1.0f,  1.0f),
    Vector3(-1.0f,  1.0f,  1.0f),
    Vector3( 1.0f,  1.0f,  1.0f),
    Vector3( 1.0f, -1.0f,  1.0f),
};

const uint8_t HEXAHEDRON_EDGES[] = {
    0, 1,
    1, 2,
    2, 3,
    3, 0,
    4, 5,
    5, 6,
    6, 7,
    7, 4,
    0, 4,
    1, 5,
    2, 6,
    3, 7
};

Mesh const g_Hexahedron(ARRAY_SIZE(HEXAHEDRON_VERTICIES), HEXAHEDRON_VERTICIES, ARRAY_SIZE(HEXAHEDRON_EDGES), HEXAHEDRON_EDGES);