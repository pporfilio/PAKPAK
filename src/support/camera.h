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

class Camera
{
public:
    virtual void mouseMove(const V2 &delta) = 0;
    virtual void mouseWheel(float delta) = 0;
    virtual V3 getPos() = 0;
    virtual Matrix4x4 getFilmToWorld(int width, int height) = 0;
    virtual V3 getLook3() = 0;
    virtual V3 getUp3() = 0;
    virtual void cameraMoveUp(bool positive) {}
    virtual void cameraMoveLook(bool positive) {}
    virtual void cameraMoveSide(bool positive) {}


    //couldn't get these to work as just variables.
    virtual float getFOVY() = 0;
    virtual float getNearClip() = 0;
    virtual float getFarClip() = 0;

    virtual int getType() = 0;

};


#endif // CAMERA_H
