#include "GameCamera.h"
#include "CS123Matrix.h"
#include "utils.h"
#include <qgl.h>


GameCamera::GameCamera() {
    reset();
}

void GameCamera::reset() {
    fovy = 60.0;
    far_clip = 1000.0;
    near_clip = 0.1;
    m_up.x = 0.0;
    m_up.y = 1.0;
    m_up.z = 0.0;

    m_pos = Vector4(.1, 4.0, -20, 1.0);

    //look toward the origin
    V3 look = -1 * getPos();
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
    m_up = rotMatY * rotMatX * m_up;
    m_look.normalize();
    m_up.normalize();

}

void GameCamera::cameraMoveUp(bool positive, bool shift_modifier) {
    float mult = shift_modifier ? .05 : .005;
    if (positive) {
        m_pos += m_up * mult * m_pos.getMagnitude();
    } else {
        m_pos -= m_up * mult * m_pos.getMagnitude();
    }
}

void GameCamera::cameraMoveLook(bool positive, bool shift_modifier) {
    float mult = shift_modifier ? .001 : .0001;
    float pos = m_pos.getMagnitude();
    if (positive) {
        m_pos += m_look * pos*pos * mult;
    } else {
        m_pos -= m_look * pos*pos * mult;
    }
}

void GameCamera::cameraMoveSide(bool positive, bool shift_modifier) {
    float mult = shift_modifier ? .05 : .005;
    if (positive) {
        m_pos -= m_look.cross(m_up) * mult * m_pos.getMagnitude();
    } else {
        m_pos += m_look.cross(m_up) * mult * m_pos.getMagnitude();
    }
}

void GameCamera::mouseWheel(float delta, bool shift_modifier)
{
    //zoom *= powf(0.999f, delta);
    if (delta > 0) {
        cameraMoveLook(true, shift_modifier);
    } else {
        cameraMoveLook(false, shift_modifier);
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

CameraState* GameCamera::getState() {
    CameraState* s = new CameraState();
    s->fovy = getFOVY();
    s->farClip = getFarClip();
    s->nearClip = getNearClip();
    s->pos = getPos();
    s->look = getLook3();
    s->up = getUp3();
    return s;
}

void GameCamera::setState(CameraState *state) {
    m_pos.x = state->pos.x;
    m_pos.y = state->pos.y;
    m_pos.z = state->pos.z;
    m_up.x = state->up.x;
    m_up.y = state->up.y;
    m_up.z = state->up.z;
    m_look.x = state->look.x;
    m_look.y = state->look.y;
    m_look.z = state->look.z;
    fovy = state->fovy;
    far_clip = state->farClip;
    near_clip = state->nearClip;
}
