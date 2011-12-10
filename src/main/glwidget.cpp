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

    m_camera = new OrbitCamera();

    F_Z3 = 0.1;

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

    cout << "Loaded framebuffer objects..." << endl;

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
    fileList.append(new QFile(tmp2.append("astra/posx.jpg").c_str()));
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("astra/negx.jpg").c_str()));
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("astra/posy.jpg").c_str()));
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("astra/negy.jpg").c_str()));
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("astra/posz.jpg").c_str()));
    tmp2 = "";
    tmp2 += tmp1;
    fileList.append(new QFile(tmp2.append("astra/negz.jpg").c_str()));
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
    tmp1 += shader_base;
    tmp2 += shader_base;
    m_shaderPrograms["reflect"] = ResourceLoader::newShaderProgram(ctx, tmp1.append("reflect.vert").c_str(),
                                                                        tmp2.append("reflect.frag").c_str());
    tmp1 = "";
    tmp2 = "";
    tmp1 += shader_base;
    tmp2 += shader_base;
    m_shaderPrograms["refract"] = ResourceLoader::newShaderProgram(ctx, tmp1.append("refract.vert").c_str(),
                                                                        tmp2.append("refract.frag").c_str());
    tmp1 = "";
    tmp1 += shader_base;
    m_shaderPrograms["brightpass"] = ResourceLoader::newFragShaderProgram(ctx, tmp1.append("brightpass.frag").c_str());
    tmp1 = "";
    tmp1 += shader_base;
    m_shaderPrograms["blur"] = ResourceLoader::newFragShaderProgram(ctx, tmp1.append("blur.frag").c_str());
    tmp1 = "";
    tmp2 = "";
    tmp1 += shader_base;
    tmp2 += shader_base;
    m_shaderPrograms["fractal"] = ResourceLoader::newShaderProgram(ctx, tmp1.append("fractal.vert").c_str(),
                                                                        tmp2.append("fractal.frag").c_str());

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

      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluPerspective(m_camera->fovy, ratio, m_camera->near_clip, m_camera->far_clip);

      gluLookAt(eye.x, eye.y, eye.z, m_camera->center.x, m_camera->center.y, m_camera->center.z,
                m_camera->up.x, m_camera->up.y, m_camera->up.z);

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
    glClearColor(0,0,1, 1);


    // Update the fps
    int time = m_clock.elapsed();
    m_fps = 1000.f / (time - m_prevTime);
    m_prevTime = time;
    int width = this->width();
    int height = this->height();

    // Render the scene to a framebuffer
    applyPerspectiveCamera(width, height);

    //// Render Sky Box
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    glCallList(m_skybox);

    renderFractal();

    glDisable(GL_DEPTH_TEST);

    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);

    paintText();
}


//Set up fragment shader and render it to a quad that fills the screen
void GLWidget::renderFractal() {

    V3 pos = m_camera->getPos();

    //pass parameters to the shader
    m_shaderPrograms["fractal"]->bind();
    m_shaderPrograms["fractal"]->setUniformValue("width", this->width());
    m_shaderPrograms["fractal"]->setUniformValue("height", this->height());
    m_shaderPrograms["fractal"]->setUniformValue("world_eye", pos.x, pos.y, pos.z);
    m_shaderPrograms["fractal"]->setUniformValue("F_Z3", F_Z3);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    render3DTexturedQuad(this->width(), this->height(), true); //AIMEI

    //clean-up
    m_shaderPrograms["fractal"]->release();
}



/**
  Called when the mouse is dragged.  Rotates the camera based on mouse movement.
**/
void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    V2 pos(event->x(), event->y());
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton)
    {
        V2 delta = pos - m_prevMousePos;
        m_camera->mouseMove(delta);
    }
    m_prevMousePos = pos;
}

/**
  Record a mouse press position.
 **/
void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_prevMousePos.x = event->x();
    m_prevMousePos.y = event->y();
}

/**
  Called when the mouse wheel is turned.  Zooms the camera in and out.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        m_camera->mouseWheel(event->delta());
    }
}

/**
  Called when the GLWidget is resized.
 **/
void GLWidget::resizeGL(int width, int height)
{
    // Resize the viewport
    glViewport(0, 0, width, height);
}

void GLWidget::render3DTexturedQuad(int width, int height, bool flip) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    Matrix4x4 film_to_world = m_camera->getFilmToWorld(width, height);

    float plane_depth = 2.0;

    float aspect = (float)width/(float)height;

    float half_height = plane_depth * tan(toRadians(m_camera->fovy/2));
    float half_width = half_height * aspect;

    Vector4 plane_ul = Vector4(-half_width, half_height, plane_depth, 1);
    Vector4 plane_ll = Vector4(-half_width, -half_height, plane_depth, 1);
    Vector4 plane_lr = Vector4(half_width, -half_height, plane_depth, 1);
    Vector4 plane_ur = Vector4(half_width, half_height, plane_depth, 1);

    Vector4 t_ul = film_to_world*plane_ul;
    Vector4 t_ll = film_to_world*plane_ll;
    Vector4 t_lr = film_to_world*plane_lr;
    Vector4 t_ur = film_to_world*plane_ur;


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


/**
  Handles any key press from the keyboard
 **/
void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_S: {
        QImage qi = grabFrameBuffer(false);
        QString filter;
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png)"), &filter);
        qi.save(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + QFileInfo(fileName).baseName() + ".png", "PNG", 100);
        break;
    }
    case Qt::Key_Minus: {
        F_Z3 = max(-1, F_Z3 - .05);
        break;
    }
    case Qt::Key_Equal: {
        F_Z3 = min(1, F_Z3 + .05);
        break;
    }
}
}

/**
  Draws text for the FPS and screenshot prompt
 **/
void GLWidget::paintText()
{
    glColor3f(1.f, 1.f, 1.f);

    // Combine the previous and current framerate
    if (m_fps >= 0 && m_fps < 1000)
    {
       m_prevFps *= 0.95f;
       m_prevFps += m_fps * 0.05f;
    }

    // QGLWidget's renderText takes xy coordinates, a string, and a font
    renderText(10, 20, "FPS: " + QString::number((int) (m_prevFps)), m_font);
    renderText(10, 35, "S: Save screenshot", m_font);
    V3 pos = m_camera->getPos();
    renderText(10, 50, "eye location : (" + QString::number((double)(pos.x)) +
               ", " + QString::number((double)(pos.y)) + ", " +
               QString::number((double)(pos.x)) + ")", m_font);
}
