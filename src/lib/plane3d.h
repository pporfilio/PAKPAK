#ifndef PLANE3D_H
#define PLANE3D_H

#include "CS123Matrix.h"
#include "CS123Vector.h"
#include "aabb.h"

class Plane3D;

typedef struct sub_region {
    Plane3D *plane;
    int row;
    int col;
} sub_region_t;


class Plane3D
{
public:
    Plane3D();
    Plane3D(float left, float right, float top, float bottom, float depth);
    sub_region_t *subdivide(int subdivs);
    AABB *getAABB();
    void print();

    Vector4 m_ul;
    Vector4 m_ur;
    Vector4 m_ll;
    Vector4 m_lr;

    void transform(Matrix4x4 mat);

};


#endif // PLANE3D_H
