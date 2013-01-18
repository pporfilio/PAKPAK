/*
Authors: Parker Porfilio, Aimei Kutt, Anne Kenyon
*/

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
#include "plane3d.h"

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
    Vector3 F_reflect_channels;
    bool julia_selected;
    bool mandelbox_selected;
    bool skybox_enabled;
    bool global_ss_enabled;
    bool ss_enabled;

    int mandelbox_coloring;
    int mandelbox_colorScheme;
    bool mandelbox_fog;
    float mandelbox_itr;
    float mandelbox_epsilon;
    float mandelbox_break;
    int mandelbox_depth;

    int m_mb_width;
    int m_mb_height;

    Vector4 t_ul;
    Vector4 t_ur;
    Vector4 t_ll;
    Vector4 t_lr;

    float half_pixel_size;


    //various state
    Camera *m_gameCamera;
    Camera *m_orbitCamera;
    Camera *m_camera;

    Vector4 bg_color;
    QString recordName;

    bool m_paused;
    bool m_show_text;

    bool m_newView;
    int m_counter;
    Plane3D *m_curr_plane;
    sub_region_t *m_curr_subdivPlanes;

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

    void radioToggeled_Orbit_Cam(bool checked);
    void radioToggeled_Game_Cam(bool checked);
    void resetCurrentCamera();
    void sliderUpdate_ITR(int newValue);
    void sliderUpdate_EPS(int newValue);
    void sliderUpdate_BRK(int newValue);
    void sliderUpdate_DEP(int newValue);

    void checkRecord(bool checked);

    void setPaused(bool checked);
    void setShowText(bool checked);


    //keypress methods
    void savePicture();
    void loadCamLocation();
    void saveCamLocation();

    void savePointCloud();

protected:
    typedef void (GLWidget::*renderRegionFP)(Plane3D*, Plane3D*);

    // Overridden QGLWidget methods
    void initializeGL();
    void paintGL();

    void paintMandelboxHelper(char *srcBuffer, sub_region_t *sub_region,
                              Plane3D *full_plane, int target_width,
                              int target_height);

    void paintRegionHelper(char *srcBuffer, sub_region_t *sub_region,
                           Plane3D *full_plane, int target_width,
                           int target_height, renderRegionFP);


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
    void createFramebufferObjects(int width, int height);

    // Drawing code
    void applyPerspectiveCamera(float width, float height);
    void applyOrthogonalCamera(float width, float height);
    void renderTexturedQuad(int width, int height, bool flip);
    void render3DTexturedQuad(int width, int height, bool flip);
    void renderFractal();
    void renderMandelbox();
    void renderMandelboxRegion(Plane3D *region, Plane3D *fullFilm);
    void renderJuliaRegion(Plane3D *region, Plane3D *fullFilm);
    void paintText();



    QString myGetSaveFileName(QWidget *parent, const QString &caption, const QString &dir,
                              const QString &filter, bool save);

private:
    QTimer m_timer;
    QTime m_clock;
    int m_prevTime;
    float m_prevFps, m_fps;
    V2 m_prevMousePos;

    // Resources
    QHash<QString, QGLShaderProgram *> m_shaderPrograms; // hash map of all shader programs
    QHash<QString, QGLFramebufferObject *> m_framebufferObjects; // hash map of all framebuffer objects
    GLuint m_skybox; // skybox call list ID
    GLuint m_cubeMap; // cubeMap texture ID
    QFont m_font; // font for rendering text
    string *m_base_path;
    string *m_resources_path;

    int frameNumber;
    bool isRecording;

    int *m_frame_times;
    int m_curr_time_pos;

    //Frame buffer attempt
//    GLuint m_fbo;
//    GLuint m_fbo_colorTex;

};

#endif // GLWIDGET_H
