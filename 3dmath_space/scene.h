#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "transform.h"
#include "camera.h"
#include "shape3.h"

namespace engine3d
{
	class GameObject
	{
	public:
		Transform transform;
		Sphere body, nose;

		GameObject() :
			transform(),
			body(Vector3(0, 1, 0), 1),
			nose(Vector3(0, 1, 0.3f), 1)
		{

		}
	};

    class Scene {
    public:
        std::vector<GameObject> obj; // a set with GameObj (each with its own transform)

        //std::vector<Sphere> toWorld() const;

        void populate(int n) {
            for (int i = 0; i < n; i++) {
                GameObject someoneNew;
                someoneNew.transform.translate = Vector3::random(14) + Vector3(0, 0, 15);
                someoneNew.transform.translate.y = 0;
                obj.push_back(someoneNew);
            }
        }

        // produces a vector of spheres in world space
        std::vector<Sphere> toWorld() const {
            std::vector<Sphere> res;
            res.clear();

            for (const GameObject& g : obj) {
                res.push_back(apply(g.transform, g.nose));
                res.push_back(apply(g.transform, g.body));
            }
            return res;
        }

        std::vector<Sphere> toView(Transform camera) const;
    };

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

    const char* lighting(Versor3 normal) {
        Versor3 lightDir = Versor3(1, 2, -2).normalized();
        // lambertian
        Scalar diffuse = dot(normal, lightDir);
        if (diffuse < 0) diffuse = 0;

        return intensityToCstr(diffuse);
    }

    void rayCasting(const std::vector<Sphere>& sphereVector) {

        Camera c(2.0, 38, 38);

        //Plane aPlane( Point3(0,-1.5,0), Versor3(0,1,0) );

        std::string screenBuffer; // a string to get ready and print all at once

        for (int y = 0; y < c.pixelDimY; y++) {
            for (int x = 0; x < c.pixelDimX; x++) {
                Point3 hitPos;
                Point3 hitNorm;
                Scalar distMax = 1000.0;

                for (Sphere s : sphereVector) {
                    rayCast(c.primaryRay(x, y), s, hitPos, hitNorm, distMax);
                }

                //rayCast( c.primaryRay(x,y)  , aPlane  , hitPos , hitNorm , distMax );

                screenBuffer += lighting(hitNorm);
            }
            screenBuffer += "\n";
        }
        //system("cls");
        std::cout << screenBuffer;
    }
} // end of namespace mgd