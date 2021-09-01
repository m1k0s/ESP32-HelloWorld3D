#pragma once

#include <stdint.h>
#include "vector3.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

struct Mesh
{
    uint8_t verticiesSize;
    const Vector3* verticies;
    uint8_t edgesSize;
    const uint8_t* edges;

    Mesh(uint8_t verticiesSize, const Vector3* verticies, uint8_t edgesSize, const uint8_t* edges) :
        verticiesSize(verticiesSize),
        verticies(verticies),
        edgesSize(edgesSize),
        edges(edges)
    {
    }
};