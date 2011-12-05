#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <math.h>
#include "CS123Algebra.h"


//Compute once the value of pi to double precision and store for use elsewhere.
const double PI = 4.0 * atan(1.0);


extern double toRadians(double degrees);

extern void printMat4x4(const Matrix4x4 &m);

extern void printVec3(const Vector3 &v);

extern void printVec4(const Vector4 &v);


#endif // UTILS_H