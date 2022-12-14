#pragma once
#include "vector3.h"
#include "shape3.h"

namespace engine3d {

    class Camera {
    public:

        Scalar focal;

        int pixelDimX, pixelDimY;

        Camera(Scalar f, int sx, int sy) :focal(f), pixelDimX(sx), pixelDimY(sy) {}

        Ray primaryRay(int pixelX, int pixelY) {
            Ray r;
            r.p = Point3(0, 0, 0); // the poinf of viewis by definition on the origin
            Scalar clipX = 2.0f * pixelX / pixelDimX - 1.0;
            Scalar clipY = -2.0f * pixelY / pixelDimY + 1.0;

            r.d = Vector3(clipX, clipY, focal).normalized();

            return r;
        }

    };

}
