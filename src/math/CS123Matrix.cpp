/*!
   @file   CS123Matrix.cpp
   @author Travis Fischer (fisch0920@gmail.com)
   @date   Fall 2008

   @brief
      Provides basic functionality for a templated, arbitrarily-sized matrix.
      You will need to fill this file in for the Camtrans assignment.

**/

// CS123Matrix.h can be found in /course/cs123/include/algebra
#include <CS123Matrix.h>
#include <iostream>

//@name Routines which generate specific-purpose transformation matrices
//@{---------------------------------------------------------------------
// @returns the scale matrix described by the vector
Matrix4x4 getScaleMat(const Vector4 &v) {

    // @DONE: [CAMTRANS] Filled in.

    Matrix4x4 smat = Matrix4x4();
    smat.a = v.x;
    smat.f = v.y;
    smat.k = v.z;
    smat.p = 1;

    return smat;
}

// @returns the translation matrix described by the vector
Matrix4x4 getTransMat(const Vector4 &v) {

    // @DONE: [CAMTRANS] Filled in.

    Matrix4x4 tmat = Matrix4x4::identity();
    tmat.d = v.x;
    tmat.h = v.y;
    tmat.l = v.z;

    return tmat;
}

// @returns the rotation matrix about the x axis by the specified angle
Matrix4x4 getRotXMat (const REAL radians) {

    // @DONE: [CAMTRANS] Filled in.

    Matrix4x4 rotx = Matrix4x4();
    double tmp_cos = cos(radians);
    double tmp_sin = sin(radians);

    rotx.a = 1;
    rotx.f = tmp_cos;
    rotx.g = -tmp_sin;
    rotx.j = tmp_sin;
    rotx.k = tmp_cos;
    rotx.p = 1;

    return rotx;
}

// @returns the rotation matrix about the y axis by the specified angle
Matrix4x4 getRotYMat (const REAL radians) {

    // @DONE: [CAMTRANS] Filled in.

    Matrix4x4 roty = Matrix4x4();
    double tmp_cos = cos(radians);
    double tmp_sin = sin(radians);

    roty.a = tmp_cos;
    roty.c = tmp_sin;
    roty.f = 1;
    roty.i = -tmp_sin;
    roty.k = tmp_cos;
    roty.p = 1;

    return roty;
}

// @returns the rotation matrix about the z axis by the specified angle
Matrix4x4 getRotZMat (const REAL radians) {

    // @DONE: [CAMTRANS] Filled in.

    Matrix4x4 rotz = Matrix4x4();
    double tmp_cos = cos(radians);
    double tmp_sin = sin(radians);

    rotz.a = tmp_cos;
    rotz.b = -tmp_sin;
    rotz.e = tmp_sin;
    rotz.f = tmp_cos;
    rotz.k = 1;
    rotz.p = 1;

    return rotz;

}

// @returns the rotation matrix around the vector and point by the specified angle
Matrix4x4 getRotMat (const Vector4 &h, const Vector4 &a, const REAL lambda) {

    // @DONE: [CAMTRANS] Filled in.

    double theta = atan2(a.z, a.x);
    double phi = -atan2(a.y, sqrt(a.x*a.x + a.z*a.z));

    //want to translate by -h to move from h to the origin
    Matrix4x4 Th = getTransMat(-h);
    Matrix4x4 M1 = getRotYMat(theta);
    Matrix4x4 M2 = getRotZMat(phi);
    Matrix4x4 M3 = getRotXMat(lambda);
    Matrix4x4 M2Inv = getInvRotZMat(phi);
    Matrix4x4 M1Inv = getInvRotYMat(theta);
    Matrix4x4 ThInv = getInvTransMat(-h);

    Matrix4x4 result = ThInv * M1Inv * M2Inv * M3 * M2 * M1 * Th;

    return result;
}


// @returns the inverse scale matrix described by the vector
Matrix4x4 getInvScaleMat(const Vector4 &v) {

    // @DONE: [CAMTRANS] Filled in.
    Matrix4x4 smat = Matrix4x4();
    smat.a = 1.0/v.x;
    smat.f = 1.0/v.y;
    smat.k = 1.0/v.z;
    smat.p = 1;

    return smat;
}

// @returns the inverse translation matrix described by the vector
Matrix4x4 getInvTransMat(const Vector4 &v) {

    // @DONE: [CAMTRANS] Filled in.
    Matrix4x4 tmat = Matrix4x4::identity();
    tmat.d = -v.x;
    tmat.h = -v.y;
    tmat.l = -v.z;

    return tmat;
}

// @returns the inverse rotation matrix about the x axis by the specified angle
Matrix4x4 getInvRotXMat (const REAL radians) {

    // @DONE: [CAMTRANS] Filled in.
    Matrix4x4 rotx = Matrix4x4();
    double tmp_cos = cos(radians);
    double tmp_sin = sin(radians);

    rotx.a = 1;
    rotx.f = tmp_cos;
    rotx.g = tmp_sin;
    rotx.j = -tmp_sin;
    rotx.k = tmp_cos;
    rotx.p = 1;

    return rotx;

}

// @returns the inverse rotation matrix about the y axis by the specified angle
Matrix4x4 getInvRotYMat (const REAL radians) {

    // @DONE: [CAMTRANS] Filled in.
    Matrix4x4 roty = Matrix4x4();
    double tmp_cos = cos(radians);
    double tmp_sin = sin(radians);

    roty.a = tmp_cos;
    roty.c = -tmp_sin;
    roty.f = 1;
    roty.i = tmp_sin;
    roty.k = tmp_cos;
    roty.p = 1;

    return roty;

}

// @returns the inverse rotation matrix about the z axis by the specified angle
Matrix4x4 getInvRotZMat (const REAL radians) {

    // @DONE: [CAMTRANS] Filled in.
    Matrix4x4 rotz = Matrix4x4();
    double tmp_cos = cos(radians);
    double tmp_sin = sin(radians);

    rotz.a = tmp_cos;
    rotz.b = tmp_sin;
    rotz.e = -tmp_sin;
    rotz.f = tmp_cos;
    rotz.k = 1;
    rotz.p = 1;

    return rotz;
}

// @returns the inverse rotation matrix around the vector and
// point by the specified angle
Matrix4x4 getInvRotMat  (const Vector4 &h, const Vector4 &a, const REAL lambda) {

    // @DONE: [CAMTRANS] Filled in.
    double theta = atan2(a.z, a.x);
    double phi = -atan2(a.y, sqrt(a.x*a.x + a.z*a.z));

    //want to translate by -h to move from h to the origin
    Matrix4x4 Th = getTransMat(-h);
    Matrix4x4 M1Inv = getInvRotYMat(theta);
    Matrix4x4 M2Inv = getInvRotZMat(phi);
    Matrix4x4 M3Inv = getInvRotXMat(lambda);
    Matrix4x4 M2 = getRotZMat(phi);
    Matrix4x4 M1 = getRotYMat(theta);
    Matrix4x4 ThInv = getInvTransMat(-h);

    Matrix4x4 result = ThInv * M1Inv * M2Inv * M3Inv * M2 * M1 * Th;

    return result;
}


//@}---------------------------------------------------------------------


