#include "utils.h"
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <qgl.h>

float minN(int n, ...) {
    double tmp;
    double min;

    va_list v1;
    va_start(v1, n);

    min = va_arg(v1, double);
    for (int i = 1; i < n; i++) {
        tmp = va_arg(v1, double);
        min = tmp < min ? tmp : min;
    }

    va_end(v1);

    return (float)min;
}

float maxN(int n, ...) {
    double tmp;
    double max;

    va_list v1;
    va_start(v1, n);

    max = va_arg(v1, double);
    for (int i = 1; i < n; i++) {
        tmp = va_arg(v1, double);
        max = tmp > max ? tmp : max;
    }

    va_end(v1);

    return (float)max;
}

double toRadians(double degrees) {
    return (degrees / 180.0) * PI;
}

void printMat4x4(const Matrix4x4 &m) {
    for (int i = 0; i < 4; i++) {
        printf("\n[");
        for (int j = 0; j < 4; j++) {
            printf(" %.2f", m.data[i*4 + j]);
        }
        printf(" ]");
    }
    printf("\n\n");
}

void printVec4(const Vector4 &v) {
    printf("[ %.2f, %.2f, %.2f, %.2f ]\n", v.x, v.y, v.z, v.w);
}

void printVec3(const Vector3 &v) {
    printf("[ %.2f, %.2f, %.2f ]\n", v.x, v.y, v.z);
}

void printV3(const V3 &v) {
    printf("[ %.2f, %.2f, %.2f ]\n", v.x, v.y, v.z);
}

CameraState* readCameraState(FILE* f) {
    CameraState *s = new CameraState();
    char boundary;
    fscanf(f, " %c", &boundary);
    if (boundary != '#') {
        printf("reading camera state failed. Was looking for a # following whitespace. Instead found %c\n", boundary);
        return NULL;
    }

    float tmp[12];
    int result;

    for (int i = 0; i < 12; i++) {

        result = fscanf(f, "%f", &tmp[i]);

        if (result != 1) {
            printf("error reading float\n");
        }

        result = fscanf(f, "%c", &boundary);
        if (result != 1 || (boundary != ',' && boundary != ';')) {
            printf("reading camera state failed. Was expecting comma or semicolon but found %c\n", boundary);
            return NULL;
        }

    }

    s->pos.x = tmp[0];
    s->pos.y = tmp[1];
    s->pos.z = tmp[2];
    s->up.x = tmp[3];
    s->up.y = tmp[4];
    s->up.z = tmp[5];
    s->look.x = tmp[6];
    s->look.y = tmp[7];
    s->look.z = tmp[8];
    s->farClip = tmp[9];
    s->nearClip = tmp[10];
    s->fovy = tmp[11];

    return s;
}

void writeCameraState(FILE* f, Camera* c) {
    CameraState *s = c->getState();
    //newline and pound sign to begin the data
    fprintf(f, "\n#");

    //write data
    fprintf(f, "%f,%f,%f;", s->pos.x, s->pos.y, s->pos.z);
    fprintf(f, "%f,%f,%f;", s->up.x, s->up.y, s->up.z);
    fprintf(f, "%f,%f,%f;", s->look.x, s->look.y, s->look.z);
    fprintf(f, "%f,%f,%f;", s->farClip, s->nearClip, s->fovy);
}

/* dst_arr must be large enough to accept all the characters in qstr
    Fills dst_arr with qstr's character data.
*/
void QStringToChar(QString *qstr, char *dst_arr) {
    QByteArray ba = qstr->toLocal8Bit();
    const char *c_str = ba.data();
    int len = ba.size() + 1; //+1 for null terminating character
    for (int i = 0; i < len; i++) {
        dst_arr[i] = c_str[i];
    }
}


//given a center, a radius, and a number n, return n evenly spaced points
//in the X-Z plane at height Y.
std::vector<V3> getPointsOnCircle(V3 center, double radius, int n) {
    std::vector<V3> points;
    for (int i = 0; i < n; i++) {
        double theta = 2 * i * PI / n;
        //printf("theta = %.2f\n", theta);
        points.push_back(V3(center.x + radius * cos(theta),
                            center.y,
                            center.z + radius * sin(theta)));
    }
    return points;
}



std::vector<V3> getPointsOnSphere(V3 center, double radius,
                                    int n_sides, int n_vertical_divisions) {
    V3 top = V3(center.x, center.y + radius, center.z);
    V3 bottom = V3(center.x, center.y - radius, center.z);

    printV3(top);
    printV3(bottom);

    std::vector<V3> points;
    points.push_back(top);
    points.push_back(bottom);

    std::vector<double> radii;
    std::vector<V3> heights;

    double theta = PI / n_vertical_divisions;

    for (int i = 1; i < n_vertical_divisions; i++) {
        radii.push_back(sin(theta*i)*radius);
        heights.push_back(V3(center.x,
                               center.y + cos(theta*i)*radius,
                               center.z));
        //same radius but negative height
        radii.push_back(sin(theta*i)*radius);
        heights.push_back(V3(center.x,
                               center.y - cos(theta*i)*radius,
                               center.z));
    }

    int len = heights.size();
    for (int i = 1; i < len; i++) {
        //printV3(heights[i]);
        //printf("%.3f\n", radii[i]);
        std::vector<V3> points_around = getPointsOnCircle(heights[i], radii[i], n_sides);
        int size = points_around.size();
        for (int i = 0; i < size; i++) {
            //printV3(points_around[i]);
            points.push_back(points_around[i]);
        }
    }
    return points;
}


std::vector<V3> computePointCloud(float step) {
//    std::vector<V3> points_on_sphere = getPointsOnSphere(V3(0, 0, 0),
//                                                           11,
//                                                           100,
//                                                           100);
    std::vector<V3> pc;
//    int len = points_on_sphere.size();
//    printf("%d\n", len);

//    for (int i = 0; i < len; i++) {
//        //cast ray from point on sphere to origin
//        //printV3(points_on_sphere[i]);
//        V3 ray = - points_on_sphere[i];
//        V3 intersection = intersectMandelbox(ray, points_on_sphere[i]);
//        if (intersection.x < -1000) {
//            continue;
//        }
//        pc.push_back(intersection);
//    }


    for (float x = -8.0; x < 8.0; x += step) {
        for (float y = -8.0; y < 8.0; y += step) {
            for (float z = -8.0; z < 8.0; z += step) {
                if (isInMandelbox2(V3(x, y, z))) {
                    pc.push_back(V3(x, y, z));
                }
            }
        }
    }


//    for (float x = 0; x < 8.0; x += step) {
//        for (float y = -8.0; y < 0; y += step) {
//            for (float z = 0; z < 8.0; z += step) {
//                if (isInMandelbox2(V3(x, y, z))) {
//                    pc.push_back(V3(x, y, z));
//                }
//            }
//        }
//    }


    return pc;
}




void writePointCloud(FILE* f, std::vector<V3> pc_vec, float step) {
    int len = pc_vec.size();
    fprintf(f, "%f\n", step);
    for (int i = 0; i < len; i++) {
        V3 v = pc_vec[i];
        fprintf(f, "%f, %f, %f\n", v.x, v.y, v.z);
    }
}


//blog.hvidtfeldts.net
void sphereFold(V3 *z, float &dz) {

    float r2 = z->dot(*z);

    if (r2 < minRadius2) {
            // linear inner scaling
            float temp = (fixedRadius2/minRadius2);
            *z *= temp;
            dz *= temp;
    } else if (r2 < fixedRadius2) {
            // this is the actual sphere inversion
            float temp = (fixedRadius2 / r2);
            *z *= temp;
            dz *= temp;
    }
}


V3 clamp(V3 v, double min, double max) {

    v.x = v.x < min ? min : (v.x > max ? max : v.x);
    v.y = v.y < min ? min : (v.y > max ? max : v.y);
    v.z = v.z < min ? min : (v.z > max ? max : v.z);
    return v;
}

void boxFold(V3 *z) {
    *z = clamp(*z, -foldingLimit, foldingLimit) * 2.0 - *z;
}

bool isInMandelbox(V3 curr_point, float *dist) {
    V3 Zn = V3(0, 0, 0);
    float dZn = 1.0;
    V3 offset = curr_point;

    bool foundFractal = true;

    for (int i = 0; i < DEPTH; i++) {

        boxFold(&Zn);
        sphereFold(&Zn, dZn);

        Zn = Scale * Zn + offset;
        dZn = Scale * dZn + 1.0;

        if (Zn.dot(Zn) > BREAK) {
            foundFractal = false;
            break;
        }
    }
    *dist = Zn.length() * log((Zn.length())) / fabs(dZn);
    return foundFractal;
}

bool isInMandelbox2(V3 curr_point) {
    V3 Zn = V3(0, 0, 0);
    float dZn = 1.0;

    bool foundFractal = true;

    for (int i = 0; i < DEPTH; i++) {

        boxFold(&Zn);
        sphereFold(&Zn, dZn);

        Zn = Scale * Zn + curr_point;
        dZn = Scale * dZn + 1.0;

        if (Zn.dot(Zn) > BREAK) {
            foundFractal = false;
            break;
        }
    }
    return foundFractal;
}



V3 intersectMandelbox(V3 ray, V3 start_point) {
    float t = 0.0;

    V3 curPoint;
    //printV3(start_point);

    curPoint.x = start_point.x;
    curPoint.y = start_point.y;
    curPoint.z = start_point.z;

    float dist = 0.0;
    float curDist = 0.0;

    for (int i = 0; i < int(ITR); i++) {

        dist += t;

        if (isInMandelbox(curPoint, &curDist)) {
            return curPoint;
        }

        t = max(curDist, float(EPS));
        curPoint.x += float(t) * ray.x;
        curPoint.y += float(t) * ray.y;
        curPoint.z += float(t) * ray.z;

    }

    //didn't intersect, total hack
    return V3(-1001, -1001, -1001);
}














