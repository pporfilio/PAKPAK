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
