#include <iostream>
#include <chrono>

#include "vector3.h"
#include "shape3.h"
#include "camera.h"
#include "quaternion.h"
#include "transform.h"
#include "scene.h"


using namespace engine3d;

int main() {
    Scene s;
    s.populate(30);
    
    rayCasting(s.toWorld());
}
