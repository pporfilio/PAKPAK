#include "aabb.h"
#include "stdio.h"

AABB::AABB(float minX, float maxX, float minY, float maxY, float minZ, float maxZ)
{
    m_minX = minX;
    m_minY = minY;
    m_minZ = minZ;
    m_maxX = maxX;
    m_maxY = maxY;
    m_maxZ = maxZ;
}


float AABB::getMaxX() {
    return m_maxX;
}

float AABB::getMaxY() {
    return m_maxY;
}

float AABB::getMaxZ() {
    return m_maxZ;
}

float AABB::getMinX() {
    return m_minX;
}

float AABB::getMinY() {
    return m_minY;
}

float AABB::getMinZ() {
    return m_minZ;
}

float AABB::getDepth() {
    return m_maxZ - m_minZ;
}

float AABB::getHeight() {
    return m_maxY - m_minY;
}

float AABB::getWidth() {
    return m_maxX - m_minX;
}

void AABB::print() {
    printf("MinX = %.2f, MaxX = %.2f, MinY = %.2f, MaxY = %.2f, MinZ = %.2f,"
           " MaxZ = %.2f\n", m_minX, m_maxX, m_minY, m_maxY, m_minZ, m_maxZ);
}
