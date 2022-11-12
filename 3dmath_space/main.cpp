#include <iostream>
#include <chrono>

#include "vector3.h"
#include "shape3.h"
#include "camera.h"


using namespace mgd;

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


// ascii art: convert an intensity value (0 to 1) into a sequence of two chars
const char* intensityToCstr(Scalar intensity) {
    switch (int(round(intensity * 8))) {
    case 0: return "  "; // darkest
    case 1: return " '";
    case 2: return " +";
    case 3: return " *";
    case 4: return " #";
    case 5: return "'#";
    case 6: return "+#";
    case 7: return "*#";
    case 8: return "##"; // lightest
    default:
    case 10: return "##";
    }
}

float currentTime() {
    static float now = 0.0;
    now += 0.005;
    return now;
    //return std::chrono::system_clock().now();
}

const char* lighting(Versor3 normal) {
    Versor3 lightDir = Versor3(1, 2, -2).normalized();
    // lambertian
    Scalar diffuse = dot(normal, lightDir);
    if (diffuse < 0) diffuse = 0;

    return intensityToCstr(diffuse);
}

void rayCastingSphere() {

    float time = currentTime();

    //lightDir = Versor3(1,2,3*std::cos(time)).normalized();

    Camera c(2.0, 38, 38);
    Sphere aSphere0(Point3(0, 0, 6), 1.5);
    Sphere aSphere1(Point3(2.0 * std::cos(time), 1, 6 + 2.0 * std::sin(time)), 1);
    Plane aPlane(Point3(0, -1.5, 0), Versor3(0, 1, 0));


    std::string screenBuffer; // a string to get ready and print all at once

    for (int y = 0; y < c.pixelDimY; y++) {
        for (int x = 0; x < c.pixelDimX; x++) {
            Point3 hitPos;
            Point3 hitNorm;
            Scalar distMax = 1000.0;
            rayCast(c.primaryRay(x, y), aSphere0, hitPos, hitNorm, distMax);
            rayCast(c.primaryRay(x, y), aSphere1, hitPos, hitNorm, distMax);
            rayCast(c.primaryRay(x, y), aPlane, hitPos, hitNorm, distMax);

            screenBuffer += lighting(hitNorm);
        }
        screenBuffer += "\n";
    }
    //system("cls");
    std::cout << screenBuffer;
}

void examplesOfSyntax() {
    Vector3 v(0, 2, 3);

    Vector3 r = v * 4;  //Vector3 r = v.operator *(4);
    r *= 0.25;
    Vector3 w = v + r; //  Vector3 w = v.operator + (r);
    v += w;

    v = r - v;
    v = -w + v;
    Scalar k = dot(v, w);

    v.x = 0.4f;

    Scalar h = v[0];

    // v[6] = 0.3;
}

int main() {
    unitTestLinearOps();
    unitTestProdutcs();
    unitTestRaycasts();
    unitTestRaycastPlane();
    while (1)
        rayCastingSphere();
}
