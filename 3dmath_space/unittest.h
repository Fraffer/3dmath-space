#pragma once

#include "vector3.h"
#include "shape3.h"
#include "transform.h"
#include "quaternion.h"

namespace engine3d
{
    void performGlobalUnitTests()
    {
        unitTestLinearOps();
        unitTestProdutcs();
        unitTestRaycasts();        
        unitTestRaycastPlane();
        unitTestQuaternion();
        unitTestTransformation();        
    }

    void unitTestLinearOps() {

        Vector3 a(24, 1, 32), b(12, 3, 54);
        Scalar k = 5.0;
        assert(areEqual(a + b, b + a));
        assert(areEqual((a + b) * k, b * k + a * k));
    }

    void unitTestProdutcs() {
        Vector3 a(24, -3, 32), b(12, 3, -54), c(10, 13, -43);
        Scalar k = 0.4f;

        assert(isZero(dot(a, cross(a, b))));
        assert(isZero(dot(b, cross(a, b))));

        assert(isZero(cross(a, b) + cross(b, a)));

        assert(areEqual(dot(k * a, b), dot(b, k * a)));
        assert(areEqual(cross(a, b + 4 * c), cross(a, b) + 4 * cross(a, c)));
        assert(areEqual(cross(2 * a, 2 * b), 4 * cross(a, b)));
    }

    void unitTestRaycasts() {
        Ray r(Point3(0, 0, 0), Vector3(1, 0, 0));
        Sphere s(Point3(5, 0, 0), 3);
        Point3 hitPoint;
        Point3 hitNorm;
        Scalar distMax = 100000;

        bool isHit = rayCast(r, s, hitPoint, hitNorm, distMax);
        assert(isHit);
        assert(areEqual(hitPoint, Point3(2, 0, 0)));
    }

    void unitTestRaycastPlane() {
        Ray r(Point3(0, 0, 0), Vector3(1, 0, 0));
        Plane p(Point3(10, 0, 0), Vector3(-1, 0, 0));
        Point3 hitPoint;
        Point3 hitNorm;
        Scalar distMax = 100000;

        bool isHit = rayCast(r, p, hitPoint, hitNorm, distMax);
        assert(isHit);
        assert(areEqual(hitPoint, Point3(10, 0, 0)));
    }

    void unitTestQuaternion()
    {
        {
            Quaternion rot = Quaternion::fromAngleAxis(180, Vector3(0, 1, 0));
            Vector3 p(0, 0, 1);
            Vector3 q = rot.apply(p);

            assert(areEqual(q, Vector3(0, 0, -1)));
        }

        {
            Quaternion rot = Quaternion::fromAngleAxis(30, Vector3(1, 1, 3));
            Vector3 p(3, 5, 6), q = p;
            for (int k = 0; k < 12; k++) q = rot.apply(q);
            assert(areEqual(q, p));
        }
    }

    void unitTestTransformation()
    {
        Transform t;
        t.rotate = Quaternion::fromAngleAxis(43.4f, Vector3(1, -3, -2));
        t.translate = Vector3(1, 3, 4);
        t.scale = 5;

        Point3 p(4, 10, -13);

        Point3 q = t.transformPoint(p);
        Point3 r = t.inverse().transformPoint(q);

        assert(areEqual(p, r));

        Transform ti = t;
        ti.invert();
        r = ti.transformPoint(q);
        assert(areEqual(p, r));

        Transform tA;
        tA.rotate = Quaternion::fromAngleAxis(-13.4f, Vector3(13, 4, 0));
        tA.translate = Vector3(0, -1, 01);
        tA.scale = 0.23;

        Transform tB = t;

        Transform tAB = tA * tB;
        assert(areEqual(
            tAB.transformPoint(p),
            tA.transformPoint(tB.transformPoint(p))
        )
        );
    }

}