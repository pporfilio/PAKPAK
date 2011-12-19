/*
Authors: Aimei Kutt, Parker Porfilio, Anne Kenyon
*/

#include "glwidget.h"
#include <iostream>
#include <QFileDialog>
#include <QGLFramebufferObject>
#include <QGLShaderProgram>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QWheelEvent>
#include "glm.h"
#include "utils.h"

#include <sstream>


using std::cout;
using std::endl;

extern "C"
{
    extern void /*APIENTRY*/ glActiveTexture(GLenum);
}

static const int MAX_FPS = 120;

/**
  Constructor.  Initialize all member variables here.
 **/
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent),
    m_timer(this), m_prevTime(0), m_prevFps(0.f), m_fps(0.f),
    m_font("Deja Vu Sans Mono", 8, 4)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_gameCamera = new GameCamera();
    m_orbitCamera = new OrbitCamera();
    m_camera = m_orbitCamera;

    F_Z3 = 0.0;
    F_C = Vector4(-.1, .1, .5, -.6);
    F_specular = true;
    F_reflect = true;

    F_reflect_channels = Vector3(1.0, 1.0, 1.0);
    julia_selected = true;
    mandelbox_selected = false;
    skybox_enabled = true;

    mandelbox_coloring = 1;
    mandelbox_colorScheme = 1;
    mandelbox_fog = true;
    mandelbox_itr = 1000.0;
    mandelbox_epsilon = 1.0/100000.0;
    mandelbox_break = 200.0;
    mandelbox_depth = 14;

    bg_color = Vector4(1.0,1.0,1.0,1.0);


    //global as in regardless of whether or not the user is clicking
    global_ss_enabled = true;
    ss_enabled = true;

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));

    /* hack because I don't know the right way to do this in C/C++ */

    string path1 = "../src/shaders/fractal.frag";
    string path2 = "../../../../src/shaders/fractal.frag";
    FILE* f1 = fopen(path1.c_str(), "r");
    FILE* f2 = fopen(path2.c_str(), "r");

    if (f1) {
        m_base_path = new string("../");
        fclose(f1);
    } else if (f2) {
        m_base_path = new string("../../../../");
    } else {
        printf("cannot find fractal fragment shader!\n");
    }

    if (f2) {
        fclose(f2);
    }

    frameNumber = 0;
    isRecording = false;

}

/**
  Destructor.  Delete any 'new'ed objects here.
 **/
GLWidget::~GLWidget()
{
    foreach (QGLShaderProgram *sp, m_shaderPrograms)
        delete sp;

    glDeleteLists(m_skybox, 1);
    const_cast<QGLContext *>(context())->deleteTexture(m_cubeMap);
    delete m_camera;
}

/**
  Initialize the OpenGL state and start the drawing loop.
 **/
void GLWidget::initializeGL()
{
    // Set up OpenGL
    glEnable(GL_TEXTURE_2D);

    glFrontFace(GL_CCW);

    glDisable(GL_DITHER);

    glDisable(GL_LIGHTING);
    glShadeModel(GL_FLAT);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Load resources, including creating shader programs and framebuffer objects
    initializeResources();

    // Start the drawing timer
    m_timer.start(1000.0f / MAX_FPS);
}

/**
  Initialize all resources.
  This includes models, textures, call lists, shader programs, and framebuffer objects.
 **/
void GLWidget::initializeResources()
{
    cout << "Using OpenGL Version " << glGetString(GL_VERSION) << endl << endl;
    // Ideally we would now check to make sure all the OGL functions we use are supported
    // by the video card.  But that's a pain to do so we're not going to.
    cout << "--- Loading Resources ---" << endl;

    string tmp = "";
    cout << m_base_path->data() << endl;

    tmp += m_base_path->data();

    cout << tmp << endl;

    tmp += "resources/";

    cout << tmp << endl;

    m_skybox = ResourceLoader::loadSkybox();
    cout << "Loaded skybox..." << endl;

    loadCubeMap();
    cout << "Loaded cube map..." << endl;

    createShaderPrograms();
    cout << "Loaded shader programs..." << endl;

    cout << " --- Finish Loading Resources ---" << endl;
}

/**
  Load a cube map for the skybox
 **/
void GLWidget::loadCubeMap()
{
    string tmp1 = "";
    tmp1 += m_base_path->data();
    tmp1 += "resources/textures/";
    string tmp2 = "";
    tmp2 += tmp1;
    QList<QFile *> fileList;

    fileList.append(new QFile(tmp2.append("stars/Galaxy_RT.jpg").c_str())); //posx
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("stars/Galaxy_LT.jpg").c_str())); //negx
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("stars/Galaxy_UP.jpg").c_str())); //posy
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("stars/Galaxy_DN.jpg").c_str())); //negy
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("stars/Galaxy_FT.jpg").c_str())); //posz
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("stars/Galaxy_BK.jpg").c_str())); //negz
    m_cubeMap = ResourceLoader::loadCubeMap(fileList);
}

/**
  Create shader programs.
 **/
void GLWidget::createShaderPrograms()
{
    string shader_base = "";
    shader_base += m_base_path->c_str();
    shader_base += "src/shaders/";

    string tmp1 = "";
    string tmp2 = "";

    const QGLContext *ctx = context();

    tmp1 = "";
    tmp2 = "";
    tmp1 += shader_base;
    tmp2 += shader_base;
    m_shaderPrograms["fractal"] = ResourceLoader::newShaderProgram(ctx, tmp1.append("fractal.vert").c_str(),
                                                                        tmp2.append("fractal.frag").c_str());
    tmp1 = "";
    tmp2 = "";
    tmp1 += shader_base;
    tmp2 += shader_base;
    m_shaderPrograms["mandelbox"] = ResourceLoader::newShaderProgram(ctx, tmp1.append("mandelbox.vert").c_str(),
                                                                        tmp2.append("mandelbox.frag").c_str());
}


/**
  Called to switch to a perspective OpenGL camera.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyPerspectiveCamera(float width, float height)
{
      float ratio = ((float) width) / height;
      V3 eye(m_camera->getPos());
      V3 looking_at = eye + m_camera->getLook3();

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(m_camera->getFOVY(), ratio, m_camera->getNearClip(),
                     m_camera->getFarClip());

      gluLookAt(eye.x, eye.y, eye.z, looking_at.x, looking_at.y, looking_at.z,
                m_camera->getUp3().x, m_camera->getUp3().y, m_camera->getUp3().z);

      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
}

/**
  Draws the scene to a buffer which is rendered to the screen when this function exits.
 **/
void GLWidget::paintGL()
{
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if ((julia_selected)&&(!skybox_enabled)) {
        bg_color = Vector4(.4,.5,.6,1.0);
    }
    else {
        if (mandelbox_colorScheme == 3) {
            bg_color = Vector4(1.0, 1.0, 0., 1.0);
        }
        else {
            bg_color = Vector4(1.0,1.0,1.0,1.0);
        }

    }
    glClearColor(bg_color.x,bg_color.y,bg_color.z, 1);


    // Update the fps
    int time = m_clock.elapsed();
    m_fps = 1000.f / (time - m_prevTime);
    m_prevTime = time;
    int width = this->width();
    int height = this->height();


    Matrix4x4 film_to_world = m_camera->getFilmToWorld(width, height);

    float plane_depth = 2.0;

    float aspect = (float)width/(float)height;

    float half_height = plane_depth * tan(toRadians(m_camera->getFOVY()/2));
    float half_width = half_height * aspect;

    Vector4 plane_ul = Vector4(-half_width, half_height, plane_depth, 1);
    Vector4 plane_ll = Vector4(-half_width, -half_height, plane_depth, 1);
    Vector4 plane_lr = Vector4(half_width, -half_height, plane_depth, 1);
    Vector4 plane_ur = Vector4(half_width, half_height, plane_depth, 1);



    t_ul = film_to_world*plane_ul;
    t_ll = film_to_world*plane_ll;
    t_lr = film_to_world*plane_lr;
    t_ur = film_to_world*plane_ur;

    //used for supersampling
    half_pixel_size = .5*(t_ur - t_ul).getMagnitude()/float(width);


    applyPerspectiveCamera(width, height);

    //// Render Sky Box
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);

    if (mandelbox_selected) {
        skybox_enabled = false;
    }

    if (skybox_enabled) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
        glCallList(m_skybox);
    }

    if (julia_selected) {
        renderFractal();
    } else if (mandelbox_selected) {
        renderMandelbox();
    }

    glDisable(GL_DEPTH_TEST);

    if (skybox_enabled) {
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glDisable(GL_TEXTURE_CUBE_MAP);
    }

    paintText();

    if (isRecording) {
        savePicture();
        frameNumber++;
    }
}


//Set up fragment shader and render it to a quad that fills the screen
void GLWidget::renderFractal() {

    V3 pos = m_camera->getPos();

    //pass parameters to the shader
    m_shaderPrograms["fractal"]->bind();
    m_shaderPrograms["fractal"]->setUniformValue("skybox_enabled", skybox_enabled ? 1 : 0);
    m_shaderPrograms["fractal"]->setUniformValue("CubeMap", GL_TEXTURE0);
    m_shaderPrograms["fractal"]->setUniformValue("width", this->width());
    m_shaderPrograms["fractal"]->setUniformValue("height", this->height());
    m_shaderPrograms["fractal"]->setUniformValue("world_eye", pos.x, pos.y, pos.z);
    m_shaderPrograms["fractal"]->setUniformValue("halfPix", half_pixel_size);
    m_shaderPrograms["fractal"]->setUniformValue("F_Z3", F_Z3);
    m_shaderPrograms["fractal"]->setUniformValue("F_C", F_C.x, F_C.y, F_C.z, F_C.w);
    m_shaderPrograms["fractal"]->setUniformValue("reflections_enabled", F_reflect ? 1 : 0);
    m_shaderPrograms["fractal"]->setUniformValue("specular_enabled", F_specular ? 1 : 0);
    m_shaderPrograms["fractal"]->setUniformValue("ss_enabled", ss_enabled ? 1 : 0);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    render3DTexturedQuad(this->width(), this->height(), true); //AIMEI

    m_shaderPrograms["fractal"]->release();
}

//Set up fragment shader and render it to a quad that fills the screen
void GLWidget::renderMandelbox() {

    V3 pos = m_camera->getPos();

    //pass parameters to the shader
    m_shaderPrograms["mandelbox"]->bind();
    m_shaderPrograms["mandelbox"]->setUniformValue("width", this->width());
    m_shaderPrograms["mandelbox"]->setUniformValue("height", this->height());
    m_shaderPrograms["mandelbox"]->setUniformValue("world_eye", pos.x, pos.y, pos.z);
    m_shaderPrograms["mandelbox"]->setUniformValue("halfPix", half_pixel_size);
    m_shaderPrograms["mandelbox"]->setUniformValue("specular_enabled", F_specular ? 1 : 0);
    m_shaderPrograms["mandelbox"]->setUniformValue("ss_enabled", ss_enabled ? 1 : 0);
    m_shaderPrograms["mandelbox"]->setUniformValue("coloring", mandelbox_coloring);
    m_shaderPrograms["mandelbox"]->setUniformValue("colorScheme", mandelbox_colorScheme);
    m_shaderPrograms["mandelbox"]->setUniformValue("fog_enabled", mandelbox_fog);
    m_shaderPrograms["mandelbox"]->setUniformValue("ITR", mandelbox_itr);
    m_shaderPrograms["mandelbox"]->setUniformValue("EPSILON", mandelbox_epsilon);
    m_shaderPrograms["mandelbox"]->setUniformValue("BREAK", mandelbox_break);
    m_shaderPrograms["mandelbox"]->setUniformValue("DEPTH", mandelbox_depth);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    render3DTexturedQuad(this->width(), this->height(), true); //AIMEI

    //clean-up
    m_shaderPrograms["mandelbox"]->release();
}


/**
  Called when the mouse is dragged.  Rotates the camera based on mouse movement.
**/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{

    V2 pos(event->x(), event->y());
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        ss_enabled = false;
        V2 delta = pos - m_prevMousePos;
        m_camera->mouseMove(delta);
    }
    else {
        ss_enabled = global_ss_enabled;
    }
    m_prevMousePos = pos;


}

/**
  Record a mouse press position.
 **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    ss_enabled = false;
    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *) {
    ss_enabled = global_ss_enabled;
}

/**
  Called when the mouse wheel is turned.  Zooms the camera in and out.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
    ss_enabled = false;
    if (event->orientation() == Qt::Vertical)
    {
        m_camera->mouseWheel(event->delta(), (event->modifiers() == Qt::ShiftModifier));
    }
}


/**
  Called when the GLWidget is resized.
 **/
void GLWidget::resizeGL(int width, int height)
{
    // Resize the viewport
    glViewport(0, 0, width, height);
    ss_enabled = false;
}

void GLWidget::render3DTexturedQuad(int width, int height, bool flip) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, flip ? 1.0f : 0.0f);
    glVertex3f(t_ul.x, t_ul.y, t_ul.z);

    glTexCoord2f(1.0f, flip ? 1.0f : 0.0f);
    glVertex3f(t_ll.x, t_ll.y, t_ll.z);


    glTexCoord2f(1.0f, flip ? 0.0f : 1.0f);
    glVertex3f(t_lr.x, t_lr.y, t_lr.z);


    glTexCoord2f(0.0f, flip ? 0.0f : 1.0f);
    glVertex3f(t_ur.x, t_ur.y, t_ur.z);
    glEnd();
}


void GLWidget::savePicture() {
    QImage qi = grabFrameBuffer(false);
    QString filter;

    std::string s;
    std::stringstream out;
    out << frameNumber;
    s = out.str();
    std::string file = "/home/BLAH/fractal_frames/fractal_" + s + ".png";  //CHANGE THIS
    QString fileName = QString::fromStdString(file);
    qi.save(fileName, "PNG", 100);

    //QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png)"), &filter);
    //qi.save(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + QFileInfo(fileName).baseName() + ".png", "PNG", 100);
}

void GLWidget::loadCamLocation() {
    string path = "";
    path += m_base_path->data();
    path += "resources/cameraData.txt";
    FILE *f = fopen(path.c_str(), "r");
    if (f) {
        readCameraState(f);
    } else {
        printf("could not open file at %s\n", path.c_str());
    }
}

/**
  Draws text for the FPS and screenshot prompt
 **/
void GLWidget::paintText()
{
    /*
    glColor3f(1.f, 1.f, 1.f);

    // Combine the previous and current framerate
    if (m_fps >= 0 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "FPS: " + QString::number((int) (m_prevFps)), m_font);
    renderText(10, 35, "P: Save screenshot", m_font);
    V3 pos = m_camera->getPos();
    renderText(10, 50, "eye location : (" + QString::number((double)(pos.x)) +
               ", " + QString::number((double)(pos.y)) + ", " +
               QString::number((double)(pos.z)) + ")", m_font);
//    renderText(10, 65, "DEPTH = " + QString::number((double)(10 / max(pos.lengthSquared(), 1.0))), m_font);
    renderText(10, 65, "DEPTH = " + QString::number((double)(pos.length())), m_font);
    renderText(10, 80, "step size = " + QString::number((double)(.05*pos.length())), m_font);
    */

}

void GLWidget::sliderUpdateF_Z3(int newValue) {
    F_Z3 = (float)newValue / 30.0;
    ss_enabled = false;
}


void GLWidget::sliderUpdateF_C_x(int newValue) {
    F_C.x = (float)newValue / 100.0;
    ss_enabled = false;
}

void GLWidget::sliderUpdateF_C_y(int newValue) {
    F_C.y = (float)newValue / 100.0;
    ss_enabled = false;
}

void GLWidget::sliderUpdateF_C_z(int newValue) {
    F_C.z = (float)newValue / 100.0;
    ss_enabled = false;
}

void GLWidget::sliderUpdateF_C_w(int newValue) {
    F_C.w = (float)newValue / 100.0;
    ss_enabled = false;
}


//fractal selection
void GLWidget::radioToggeled_Julia(bool checked) {
    julia_selected = checked;
}

void GLWidget::radioToggeled_Mandelbox(bool checked) {
    mandelbox_selected = checked;
}

void GLWidget::radioToggeled_specular(bool checked) {
    F_specular = checked;
}

void GLWidget::radioToggeled_reflect(bool checked) {
    F_reflect = checked;
}

void GLWidget::checkToggeled_skybox(bool checked) {
    skybox_enabled = checked;

}

void GLWidget::checkToggeled_ss(bool checked) {
    global_ss_enabled = checked;
    ss_enabled = global_ss_enabled;
}


void GLWidget::radioToggeled_Orbit_Cam(bool checked) {
    if (checked) {
        m_camera = m_orbitCamera;
    }
}

void GLWidget::radioToggeled_Game_Cam(bool checked) {
    if (checked) {
        m_camera = m_gameCamera;
    }
}

void GLWidget::resetCurrentCamera() {
    m_camera->reset();
}

void GLWidget::sliderUpdate_ITR(int newValue) {
    mandelbox_itr = newValue;
}

void GLWidget::sliderUpdate_EPS(int newValue) {
    mandelbox_epsilon = float(newValue)/100000.0;
}

void GLWidget::sliderUpdate_BRK(int newValue) {
    mandelbox_break = newValue;
}

void GLWidget::checkRecord(bool checked) {
    isRecording = checked;
}
