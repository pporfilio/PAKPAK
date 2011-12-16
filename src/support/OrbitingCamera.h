#ifndef ORBITINGCAMERA_H
#define ORBITINGCAMERA_H

#include <QMouseEvent>
#include "vector.h"
#include "CS123Matrix.h"
#include "utils.h"
#include "camera.h"

/**
    An orbiting perspective camera specified by a center, two angles, and a zoom factor

    @author: Justin Ardini (jardini)
**/
class OrbitCamera : public Camera
{
public:

    OrbitCamera();

    V3 center, up;
    float angle_x;
    float angle_y;
    float fovy;
    float zoom;

    float near_clip;
    float far_clip;

    void mouseMove(const V2 &delta);
    void mouseWheel(float delta, bool shift_modifier);
    V3 getPos();
    V3 getLook3();
    V3 getUp3();
    Matrix4x4 getFilmToWorld(int width, int height);
    float getFOVY();
    float getNearClip();
    float getFarClip();
    int getType();
    CameraState* getState();
    void setState(CameraState *state);

};


#endif // ORBITINGCAMERA_H
