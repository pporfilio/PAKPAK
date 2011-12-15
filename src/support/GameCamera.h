#ifndef GAMECAMERA_H
#define GAMECAMERA_H

#include <QMouseEvent>
#include "vector.h"
#include "CS123Matrix.h"
#include "utils.h"
#include "camera.h"

/**
    An orbiting perspective camera specified by a center, two angles, and a zoom factor

    @author: Justin Ardini (jardini)
**/
class GameCamera : public Camera
{
public:

    GameCamera();

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
    float getFOVY();
    float getNearClip();
    float getFarClip();
    int getType();


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

#endif // GAMECAMERA_H
