#pragma once

#include <stdint.h>
#include "mesh.h"

const Vector3 OCTAHEDRON_VERTICIES[] = {
    Vector3( 0.0,  0.0, -1.0),
    Vector3( 0.0,  0.0,  1.0),
    Vector3(-1.0,  0.0,  0.0),
    Vector3( 0.0, -1.0,  0.0),
    Vector3( 1.0,  0.0,  0.0),
    Vector3( 0.0,  1.0,  0.0)
};

const uint8_t OCTAHEDRON_EDGES[] = {
    0, 2,
    0, 3,
    0, 4,
    0, 5,
    1, 2,
    1, 3,
    1, 4,
    1, 5,
    2, 3,
    3, 4,
    4, 5,
    5, 2
};

Mesh const g_Octahedron(ARRAY_SIZE(OCTAHEDRON_VERTICIES), OCTAHEDRON_VERTICIES, ARRAY_SIZE(OCTAHEDRON_EDGES), OCTAHEDRON_EDGES);