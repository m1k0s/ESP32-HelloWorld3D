#pragma once

#include <math.h>
#include "viewport.h"

struct Vector3
{
    float x;
    float y;
    float z;

    Vector3()
    {
    }

    Vector3(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

    Vector3 RotateX(float rad) const
    {
        float cosa = cosf(rad);
        float sina = sinf(rad);
        return Vector3(x, y * cosa - z * sina, y * sina + z * cosa);
    }

    Vector3 RotateY(float rad) const
    {
        float cosa = cosf(rad);
        float sina = sinf(rad);
        return Vector3(x, y * cosa - z * sina, y * sina + z * cosa);
    }

    Vector3 RotateZ(float rad) const
    {
        float cosa = cosf(rad);
        float sina = sinf(rad);
        return Vector3(x * cosa - y * sina, x * sina + y * cosa, z);
    }

    Vector3 Project(const Viewport& viewport) const
    {
        float factor = viewport.fov / (viewport.eyeDistance + z);
        return Vector3(x * factor + viewport.halfWidth, -y * factor + viewport.halfHeight, z);
    }
};