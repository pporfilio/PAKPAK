/*
  @file camera.h
  @author Parker Porfilio
  @date Fall 2011

  @brief Contains interface fro abstract virtual camera class. Based on Camera.h
      written by Ben Herila and Evan Wallace and distributed as part of
      Brown University's CS123 support code.

  */

#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"
#include "CS123Matrix.h"

enum CameraType {
    GAME_CAMERA, ORBIT_CAMERA
};

struct CameraState {
    float fovy;
    float nearClip;
    float farClip;
    V3 pos;
    V3 look;
    V3 up;
};



class Camera
{
public:
    virtual void mouseMove(const V2 &delta) = 0;
    virtual void mouseWheel(float delta, bool shift_modifier) = 0;
    virtual V3 getPos() = 0;
    virtual Matrix4x4 getFilmToWorld(int width, int height) = 0;
    virtual V3 getLook3() = 0;
    virtual V3 getUp3() = 0;
    virtual void reset() = 0;
    virtual void cameraMoveUp(bool positive, bool shift_modifier) {}
    virtual void cameraMoveLook(bool positive, bool shift_modifier) {}
    virtual void cameraMoveSide(bool positive, bool shift_modifier) {}
    virtual void cameraRotLook(bool pos, bool shift_modifier) {}


    //couldn't get these to work as just variables.
    virtual float getFOVY() = 0;
    virtual float getNearClip() = 0;
    virtual float getFarClip() = 0;
    virtual int getType() = 0;
    virtual CameraState* getState() = 0;
    virtual void setState(CameraState *state) = 0;

};


#endif // CAMERA_H
