#ifndef AABB_H
#define AABB_H

class AABB
{
public:
//    AABB();
//    AABB(float left, float right, float bottom, float top, float back, float front);
    AABB(float minX = -1, float maxX = 1, float minY = -1, float maxY = 1,
         float minZ = -1, float maxZ = 1);

    float getMinX();
    float getMaxX();
    float getMinY();
    float getMaxY();
    float getMinZ();
    float getMaxZ();

    float getWidth();
    float getHeight();
    float getDepth();

    void print();

private:
    float m_minX;
    float m_minY;
    float m_minZ;
    float m_maxX;
    float m_maxY;
    float m_maxZ;
};

#endif // AABB_H
