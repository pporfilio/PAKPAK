#include "utils.h"
#include "CS123Algebra.h"
#include <stdio.h>
#include <math.h>
#include <qgl.h>



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

CameraState* readCameraState(FILE* f) {
    CameraState *s = new CameraState();
    char boundary;
    fscanf(f, " %c", &boundary);
    if (boundary != '#') {
        printf("reading camera state failed. Was looking for a # following whitespace. Instead found %c\n", boundary);
        return NULL;
    }
    else {
        printf("found pound sign!\n");
    }
}
