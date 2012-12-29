#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <math.h>
#include "camera.h"
//#include "math/vector.h""



//Compute once the value of pi to double precision and store for use elsewhere.
const double PI = 4.0 * atan(1.0);

const float foldingLimit = 1.0;
const int DEPTH = 14;
const float minRadius = 0.5;
const float minRadius2 = 0.5 * 0.5;
const float fixedRadius = 1.0;
const float fixedRadius2 = fixedRadius * fixedRadius;
const float Scale = 2.0;
const float BREAK = 200.0;
const float ITR = 4000.0;
const float EPS = 1.0/100000.0;

extern float maxN(int n, ...);

float minN(int n, ...);

extern CameraState* readCameraState(FILE* f);

extern void writeCameraState(FILE *f, Camera* c);

extern double toRadians(double degrees);

extern void printMat4x4(const Matrix4x4 &m);

extern void printVec3(const Vector3 &v);

extern void printVec4(const Vector4 &v);

extern void QStringToChar(QString *qstr, char *dst_arr);

extern std::vector<V3> getPointsOnCircle(V3 center, double radius, int n);

extern std::vector<V3> getPointsOnSphere(V3 center, double radius,
                                    int n_sides, int n_vertical_divisions);


extern std::vector<V3> computePointCloud(float step);

extern void writePointCloud(FILE* f, std::vector<V3> pc_vec, float step);

extern void sphereFold(V3 *z, float &dz);

extern V3 clamp(V3 v, double min, double max);

extern void boxFold(V3 *z);

extern bool isInMandelbox(V3 curr_point, float *dist);

extern bool isInMandelbox2(V3 curr_point);

extern V3 intersectMandelbox(V3 ray, V3 start_point);






#endif // UTILS_H
