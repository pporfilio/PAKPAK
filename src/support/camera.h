#ifndef CAMERA_H
#define CAMERA_H

#include <QMouseEvent>
#include "vector.h"
#include "CS123Matrix.h"
#include "utils.h"

/**
    An orbiting perspective camera specified by a center, two angles, and a zoom factor

    @author: Justin Ardini (jardini)
**/
struct OrbitCamera
{
    V3 center, up;
    float theta, phi;
    float fovy;
    float zoom;

    float near_clip;
    float far_clip;

    void mouseMove(const V2 &delta);
    void mouseWheel(float delta);
    Matrix4x4 getFilmToWorld(int width, int height);

};

#endif // CAMERA_H
