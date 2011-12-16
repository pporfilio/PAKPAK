#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui/QMainWindow>
#include <QGLWidget>
#include <QHash>
#include <QString>
#include <QTimer>
#include <QTime>

#include "mainwindow.h"
#include "camera.h"
#include "GameCamera.h"
#include "OrbitingCamera.h"
#include "CS123Matrix.h"
#include "CS123Vector.h"
#include "vector.h"
#include "resourceloader.h"

class QGLShaderProgram;
class QGLFramebufferObject;


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    //fractal parameter variables
    float F_Z3;
    Vector4 F_C;
    bool F_specular;
    bool F_reflect;
    Vector3 F_spec_channels;
    Vector3 F_reflect_channels;
    bool julia_selected;
    bool mandelbox_selected;
    bool skybox_enabled;
    bool global_ss_enabled;
    bool ss_enabled;

    Vector4 t_ul;
    Vector4 t_ur;
    Vector4 t_ll;
    Vector4 t_lr;

    float half_pixel_size;


    //various state
    Camera *m_gameCamera;
    Camera *m_orbitCamera;
    Camera *m_camera;



    //ui interaction methods
    void sliderUpdateF_Z3(int newValue);
    void sliderUpdateF_C_x(int newValue);
    void sliderUpdateF_C_y(int newValue);
    void sliderUpdateF_C_z(int newValue);
    void sliderUpdateF_C_w(int newValue);
    void radioToggeled_specular(bool checked);
    void radioToggeled_reflect(bool checked);
    void radioToggeled_Julia(bool checked);
    void radioToggeled_Mandelbox(bool checked);
    void checkToggeled_skybox(bool checked);
    void checkToggeled_ss(bool checked);
    void sliderUpdateF_spec_channels_r(int newValue);
    void sliderUpdateF_spec_channels_g(int newValue);
    void sliderUpdateF_spec_channels_b(int newValue);
    void sliderUpdateF_reflect_channels_r(int newValue);
    void sliderUpdateF_reflect_channels_g(int newValue);
    void sliderUpdateF_reflect_channels_b(int newValue);
    void radioToggeled_Orbit_Cam(bool checked);
    void radioToggeled_Game_Cam(bool checked);
    void resetCurrentCamera();

    //keypress methods
    void savePicture();
    void loadCamLocation();

protected:
    // Overridden QGLWidget methods
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void wheelEvent(QWheelEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    //void keyPressEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *);

    // Initialization code
    void initializeResources();
    void loadCubeMap();
    void createShaderPrograms();

    // Drawing code
    void applyPerspectiveCamera(float width, float height);
    void render3DTexturedQuad(int width, int height, bool flip);
    void renderFractal();
    void renderMandelbox();
    void paintText();

private:
    QTimer m_timer;
    QTime m_clock;
    int m_prevTime;
    float m_prevFps, m_fps;
    V2 m_prevMousePos;

    // Resources
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    GLuint m_skybox; // skybox call list ID
    GLuint m_cubeMap; // cubeMap texture ID
    QFont m_font; // font for rendering text
    string *m_base_path;
};

#endif // GLWIDGET_H
