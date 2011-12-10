#include "camera.h"
#include "utils.h"
#include <qgl.h>


OrbitCamera::OrbitCamera() {
    center = V3(0.0, 0.0, 0.0);
    up = V3(0.0, 1.0, 0.0);
    zoom = 3.5;
    angle_x = PI * 1.5f;
    angle_y = 0.2f;
    fovy = 60.0;
    far_clip = 1000.0;
    near_clip = 0.1;

}

void OrbitCamera::mouseMove(const V2 &delta)
{
    // move camera around the origin
    angle_x += delta.x*0.005;
    angle_y += delta.y*0.005;

    // Keep angle_x in [0, 2pi] and angle_y in [-pi/2, pi/2]
    angle_x -= floorf(angle_x / M_2PI) * M_2PI;
    angle_y = max(0.01f - M_PI / 2, min(M_PI / 2 - 0.01f, angle_y));
}

void OrbitCamera::mouseWheel(float delta)
{
    zoom *= powf(0.999f, delta);
    //maintain zoom within reason
    //zoom = max(zoom, 1.2);
    //zoom = min(zoom, 8.0);
}

V3 OrbitCamera::getPos() {
    //compute position based on angles
    V3 pos = V3::fromAngles(angle_x, angle_y);
    pos *= zoom;
    return pos;
}

Matrix4x4 OrbitCamera::getFilmToWorld(int width, int height) {

    //compute the rotation transform
    V3 pos = getPos();
    V3 dir = center - pos;
    Vector3 look = Vector3(dir.x, dir.y, dir.z);

    Vector3 w = (look / look.getMagnitude());
    Vector3 tmp_up = Vector3(up.x, up.y, up.z);
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
