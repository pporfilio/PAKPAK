#include "GameCamera.h"
#include "CS123Matrix.h"
#include "utils.h"
#include <qgl.h>


GameCamera::GameCamera() {
    V3 center = V3(0.0, 0.0, 0.0);
    V3 up = V3(0.0, 1.0, 0.0);
    zoom = 3.5;
    angle_x = PI * 1.5f;
    angle_y = 0.2f;



    fovy = 60.0;
    far_clip = 1000.0;
    near_clip = 0.1;
    m_up.x = 0.0;
    m_up.y = 1.0;
    m_up.z = 0.0;


    V3 pos = V3::fromAngles(angle_x, angle_y);
    m_pos = Vector4(pos.x, pos.y, pos.z, 1.0);
    m_pos *= zoom;


    V3 look = (center - getPos());
    m_look.x = look.x;
    m_look.y = look.y;
    m_look.z = look.z;
    m_look.normalize();

}

void GameCamera::mouseMove(const V2 &delta)
{

    Matrix4x4 rotMatX = getRotMat(Vector4(getPos().x, getPos().y, getPos().z, 1.0), m_up, -delta.x*.005);
    Matrix4x4 rotMatY = getRotMat(Vector4(getPos().x, getPos().y, getPos().z, 1.0), m_up.cross(m_look), delta.y*.005);

    m_look = rotMatY * rotMatX * m_look;
    m_look.normalize();

}

void GameCamera::cameraMoveUp(bool positive) {
    if (positive) {
        m_pos += m_up * .05 * m_pos.getMagnitude();
    } else {
        m_pos -= m_up * .05 * m_pos.getMagnitude();
    }
}

void GameCamera::cameraMoveLook(bool positive) {
    if (positive) {
        m_pos += m_look * .05 * m_pos.getMagnitude();
    } else {
        m_pos -= m_look * .05 * m_pos.getMagnitude();
    }
}

void GameCamera::cameraMoveSide(bool positive) {
    if (positive) {
        m_pos -= m_look.cross(m_up) * .05 * m_pos.getMagnitude();
    } else {
        m_pos += m_look.cross(m_up) * .05 * m_pos.getMagnitude();
    }
}

void GameCamera::mouseWheel(float delta)
{
    //zoom *= powf(0.999f, delta);
    if (delta > 0) {
        cameraMoveLook(true);
    } else {
        cameraMoveLook(false);
    }
    //maintain zoom within reason
    //zoom = max(zoom, 1.2);
    //zoom = min(zoom, 8.0);
}

V3 GameCamera::getPos() {
    //compute position based on angles
    V3 pos2 = V3(m_pos.x, m_pos.y, m_pos.z);
    return pos2;
}

V3 GameCamera::getLook3() {
    return V3(m_look.x, m_look.y, m_look.z);
}

V3 GameCamera::getUp3() {
    return V3(m_up.x, m_up.y, m_up.z);
}

float GameCamera::getFOVY() {
    return fovy;
}

float GameCamera::getFarClip() {
    return far_clip;
}

float GameCamera::getNearClip() {
    return near_clip;
}

int GameCamera::getType() {
    return GAME_CAMERA;
}

Matrix4x4 GameCamera::getFilmToWorld(int width, int height) {

    //compute the rotation transform
    V3 pos = getPos();
    V3 dir = getLook3();
    Vector3 look = Vector3(dir.x, dir.y, dir.z);

    Vector3 w = (look / look.getMagnitude());
    Vector3 tmp_up = Vector3(m_up.x, m_up.y, m_up.z);
    Vector3 u = (tmp_up.cross(w))/((tmp_up.cross(w))).getMagnitude();
    Vector3 v = w.cross(u);

    Matrix4x4 rotate = Matrix4x4::identity();
    rotate.a = u.x;
    rotate.b = u.y;
    rotate.c = u.z;
    rotate.e = v.x;
    rotate.f = v.y;
    rotate.g = v.z;
    rotate.i = w.x;
    rotate.j = w.y;
    rotate.k = w.z;

    //compute translation transform
    Matrix4x4 translate = getInvTransMat(Vector4(pos.x, pos.y, pos.z, 1));

    return (rotate * translate).getInverse();
}
