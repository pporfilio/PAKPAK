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
class OrbitCamera
{
public:

    OrbitCamera();

    //V3 center, up;
    float fovy;

    float near_clip;
    float far_clip;

    void mouseMove(const V2 &delta);
    void mouseWheel(float delta);
    V3 getPos();
    Matrix4x4 getFilmToWorld(int width, int height);
    V3 getLook3();
    V3 getUp3();
    void cameraMoveUp(bool positive);
    void cameraMoveLook(bool positive);
    void cameraMoveSide(bool positive);


private:
    float angle_x;
    float angle_y;
    float zoom;
    Vector4 m_look;
    Vector4 m_up;
    Vector4 m_u;
    Vector4 m_v;
    Vector4 m_w;
    Vector4 m_pos;

};

#endif // CAMERA_H
