#include "camera.h"
#include "util.h"
#include <qgl.h>


OrbitCamera::OrbitCamera() {
    center = V3(0.0, 0.0, 0.0);
    up = V3(0.0, 1.0, 0.0);
    zoom = 3.5;
    angle_x = PI * 1.5f;
    angle_y = 0.2f;
    theta = angle_x;
    phi = angle_y;
    fovy = 60.0;
    far_clip = 1000.0;
    near_clip = 0.1;

}

void OrbitCamera::mouseMove(const V2 &delta)
{
    // move camera around the origin
    angle_x += delta.x*0.001;
    angle_y += delta.y*0.001;

    // Keep angle_x in [0, 2pi] and angle_y in [-pi/2, pi/2]
    angle_x -= floorf(angle_x / M_2PI) * M_2PI;
    angle_y = max(0.01f - M_PI / 2, min(M_PI / 2 - 0.01f, angle_y));
    theta = angle_x;
    phi = angle_y;
}

void OrbitCamera::mouseWheel(float delta)
{
    zoom *= powf(0.999f, delta);
}

V3 OrbitCamera::getPos() {
    V3 pos = V3::fromAngles(angle_x, angle_y);
    pos *= zoom;
    return pos;
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
    V3 pos = getPos();
    V3 dir = center - pos;

    printf("%.2f\n", (pos - center).length());

    Vector3 look = Vector3(dir.x, dir.y, dir.z);

    printf("pos: ");
    printVec3(Vector3(pos.x, pos.y, pos.z));
    printf("look: ");
    printVec3(look);

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

//    Matrix4x4 rotate = Matrix4x4::identity();
//    rotate *= getRotXMat(angle_x);
//    rotate *= getRotYMat(angle_y);


    //compute translation transform
    //V3 pos = getPos();
    Matrix4x4 translate = getInvTransMat(Vector4(pos.x, pos.y, pos.z, 1));


    return (scale_clip * rotate * translate).getInverse();
}
