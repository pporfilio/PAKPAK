#include "camera.h"
#include <qgl.h>

void OrbitCamera::mouseMove(const V2 &delta)
{
    // Rotate the eye vector around the origin
    theta += delta.x * 0.01f;
    phi += delta.y * 0.01f;

    // Keep theta in [0, 2pi] and phi in [-pi/2, pi/2]
    theta -= floorf(theta / M_2PI) * M_2PI;
    phi = max(0.01f - M_PI / 2, min(M_PI / 2 - 0.01f, phi));
}

void OrbitCamera::mouseWheel(float delta)
{
    zoom *= powf(0.999f, delta);
}

Matrix4x4 OrbitCamera::getFilmToWorld(int width, int height) {

    //compute the scale/clipping matrix transform
    float height_angle = fovy / 2.0;

    Matrix4x4 scale_clip = Matrix4x4::identity();

    double tanh_tmp = tan(toRadians(height_angle)/2.0);

    double aspect_ratio = (float)width / (float)height;

    scale_clip.a = 1.0 / (aspect_ratio * tanh_tmp * far_clip);
    scale_clip.f = 1.0 / (tanh_tmp * far_clip);
    scale_clip.k = 1.0 / far_clip;


    //compute the rotation transform
    V3 dir(-V3::fromAngles(theta, phi));
    Vector3 look = Vector3(dir.x, dir.y, dir.z);
    Vector3 w = -(look / look.getMagnitude());
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
    Matrix4x4 translate = getInvTransMat(Vector4(center.x, center.y, center.z, 1));


    return (scale_clip * rotate * translate).getInverse();
}
