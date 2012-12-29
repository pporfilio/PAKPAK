#include "plane3d.h"
#include "utils.h"

//returns an almost uninitialized Plane3D
Plane3D::Plane3D()
{
    m_ul.w = 1;
    m_ur.w = 1;
    m_ll.w = 1;
    m_lr.w = 1;

}

Plane3D::Plane3D(float left, float right, float top, float bottom, float depth) {
    m_ul.x = left;
    m_ul.y = top;
    m_ul.z = depth;
    m_ul.w = 1;

    m_ur.x = right;
    m_ur.y = top;
    m_ur.z = depth;
    m_ur.w = 1;

    m_ll.x = left;
    m_ll.y = bottom;
    m_ll.z = depth;
    m_ll.w = 1;

    //printf("m_ll = (%.2f, %.2f, %.2f, %.2f)\n", m_ll.x, m_ll.y, m_ll.z, m_ll.w);

    m_lr.x = right;
    m_lr.y = bottom;
    m_lr.z = depth;
    m_lr.w = 1;
}

void Plane3D::transform(Matrix4x4 mat) {
    m_ul = mat*m_ul;
    m_ur = mat*m_ur;
    m_lr = mat*m_lr;
    m_ll = mat*m_ll;
}

sub_region_t* Plane3D::subdivide(int subdivs) {
    sub_region_t *result = (sub_region_t*)malloc(subdivs * subdivs * sizeof(sub_region_t));
    float left = m_ur.x;
    float top = m_ul.y;
    float xStep = (m_ul.x - m_ur.x) / (float)subdivs;
    //    float xStep = (m_ur.x - m_ul.x) / (float)subdivs;
    float yStep = (m_ll.y - m_ul.y) / (float)subdivs;
    //printf("left = %.2f, top = %.2f\n");
    for (int row = 0; row < subdivs; row ++) {
        float y = yStep * row;
        for (int col = 0; col < subdivs; col ++) {
            float x = xStep * col;
            result[row * subdivs + col].plane = new Plane3D(left + x, left + x + xStep,
                                                            top + y, top + y + yStep,
                                                            m_ul.z);
            result[row * subdivs + col].row = row;
            result[row * subdivs + col].col = col;
        }
    }
    return result;
}

AABB* Plane3D::getAABB() {
    return new AABB(minN(4, m_ul.x, m_ur.x, m_lr.x, m_ll.x),
                    maxN(4, m_ul.x, m_ur.x, m_lr.x, m_ll.x),
                    minN(4, m_ul.y, m_ur.y, m_lr.y, m_ll.y),
                    maxN(4, m_ul.y, m_ur.y, m_lr.y, m_ll.y),
                    minN(4, m_ul.z, m_ur.z, m_lr.z, m_ll.z),
                    maxN(4, m_ul.z, m_ur.z, m_lr.z, m_ll.z));
}

void Plane3D::print() {
    printf("ul = (%.2f, %.2f, %.2f, %.2f), ur = (%.2f, %.2f, %.2f, %.2f), "
           "ll = (%.2f, %.2f, %.2f, %.2f), lr = (%.2f, %.2f, %.2f, %.2f)\n",
           m_ul.x, m_ul.y, m_ul.z, m_ul.w, m_ur.x, m_ur.y, m_ur.z, m_ur.w,
           m_ll.x, m_ll.y, m_ll.z, m_ll.w, m_lr.x, m_lr.y, m_lr.z, m_lr.w);
}
