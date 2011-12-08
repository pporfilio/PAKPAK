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
    //test_camera = new OrbitingCamera();

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
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
        delete fbo;
    glDeleteLists(m_skybox, 1);
    const_cast<QGLContext *>(context())->deleteTexture(m_cubeMap);
    glmDelete(m_dragon.model);
    delete m_camera;
}

/**
  Initialize the OpenGL state and start the drawing loop.
 **/
void GLWidget::initializeGL()
{
    // Set up OpenGL
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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

    m_dragon = ResourceLoader::loadObjModel(tmp.append("models/xyzrgb_dragon.obj").c_str());
    cout << "Loaded dragon..." << endl;

    m_skybox = ResourceLoader::loadSkybox();
    cout << "Loaded skybox..." << endl;

    loadCubeMap();
    cout << "Loaded cube map..." << endl;

    createShaderPrograms();
    cout << "Loaded shader programs..." << endl;

    createFramebufferObjects(width(), height());
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
    tmp1 += shader_base;
    m_shaderPrograms["fractal"] = ResourceLoader::newFragShaderProgram(ctx, tmp1.append("fractal.frag").c_str());
}

/**
  Allocate framebuffer objects.

  @param width: the viewport width
  @param height: the viewport height
 **/
void GLWidget::createFramebufferObjects(int width, int height)
{
    // Allocate the main framebuffer object for rendering the scene to
    // This needs a depth attachment
    m_framebufferObjects["fbo_0"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    m_framebufferObjects["fbo_0"]->format().setSamples(16);
    // Allocate the secondary framebuffer objects for rendering textures to (post process effects)
    // These do not require depth attachments
    m_framebufferObjects["fbo_1"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    // TODO: Create another framebuffer here.  Look up two lines to see how to do this... =.=
    m_framebufferObjects["fbo_2"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
}

/**
  Called to switch to an orthogonal OpenGL camera.
  Useful for rending a textured quad across the whole screen.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyOrthogonalCamera(float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-width/2.0, width/2.0, height/2.0, -height/2.0, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
  Called to switch to a perspective OpenGL camera.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::applyPerspectiveCamera(float width, float height)
{
    float ratio = ((float) width) / height;
    V3 dir(-V3::fromAngles(m_camera->theta, m_camera->phi));
    V3 eye(m_camera->center - dir * m_camera->zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(m_camera->fovy, ratio, m_camera->near_clip, m_camera->far_clip);
    gluLookAt(eye.x, eye.y, eye.z, eye.x + dir.x, eye.y + dir.y, eye.z + dir.z,
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
    //m_framebufferObjects["fbo_0"]->bind();
    applyPerspectiveCamera(width, height);

    float gl_modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, gl_modelview);

    renderScene();
    //m_framebufferObjects["fbo_0"]->release();

    // Copy the rendered scene into framebuffer 1
    m_framebufferObjects["fbo_0"]->blitFramebuffer(m_framebufferObjects["fbo_1"],
                                                   QRect(0, 0, width, height), m_framebufferObjects["fbo_0"],
                                                   QRect(0, 0, width, height), GL_COLOR_BUFFER_BIT, GL_NEAREST);


    applyOrthogonalCamera(width, height);



    //render a quad to fill the screen
    //this fills texture0 with black in the upper left,
    //green in the upper right, and red in the lower left


    m_framebufferObjects["fbo_1"]->bind();
    renderColoredQuad(width, height, true);
    m_framebufferObjects["fbo_1"]->release();


    //get modelview transform to pass to fragment shader
    Matrix4x4 modelview = m_camera->getFilmToWorld(width, height);

    renderFractal(modelview);
    


    paintText();
}


//Set up fragment shader and render it to a quad that fills the screen
void GLWidget::renderFractal(Matrix4x4 film_to_world) {


    V3 pos = m_camera->getPos();

    float film_to_world_floats[16];
    for (int i = 0; i < 16; i++) {
        film_to_world_floats[i] = (float)film_to_world.data[i];
    }

    printf("width = %d, height = %d\n", this->width(), this->height());

    //pass parameters to the shader
    m_shaderPrograms["fractal"]->bind();
    m_shaderPrograms["fractal"]->setUniformValue("width", this->width());
    m_shaderPrograms["fractal"]->setUniformValue("height", this->height());
    m_shaderPrograms["fractal"]->setUniformValueArray("film_to_world", film_to_world_floats, 16, 1);
    m_shaderPrograms["fractal"]->setUniformValue("world_eye", pos.x, pos.y, pos.z);
    m_shaderPrograms["fractal"]->setUniformValue("F_Z3", F_Z3);

    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //provide texture with color indicating x, y pixel location to the shader
    glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());

    //render the quad (i.e. compute per-pixel in the fragment shader)

    //renderTexturedQuad(this->width(), this->height(), true);
    render3DTexturedQuad(this->width(), this->height(), true, 1); //AIMEI

    //clean-up
    m_shaderPrograms["fractal"]->release();
    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
  Renders the scene.  May be called multiple times by paintGL() if necessary.
**/
void GLWidget::renderScene() {
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox
    glEnable(GL_TEXTURE_CUBE_MAP);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
    glCallList(m_skybox);

    // Enable culling (back) faces for rendering the dragon
    glEnable(GL_CULL_FACE);

    // Render the dragon with the refraction shader bound
    glActiveTexture(GL_TEXTURE0);
    m_shaderPrograms["refract"]->bind();
    m_shaderPrograms["refract"]->setUniformValue("CubeMap", GL_TEXTURE0);
    glPushMatrix();
    glTranslatef(-1.25f, 0.f, 0.f);
    glCallList(m_dragon.idx);
    glPopMatrix();
    m_shaderPrograms["refract"]->release();

    // Render the dragon with the reflection shader bound
    m_shaderPrograms["reflect"]->bind();
    m_shaderPrograms["reflect"]->setUniformValue("CubeMap", GL_TEXTURE0);
    glPushMatrix();
    glTranslatef(1.25f,0.f,0.f);
    glCallList(m_dragon.idx);
    glPopMatrix();
    m_shaderPrograms["reflect"]->release();

    // Disable culling, depth testing and cube maps
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_CUBE_MAP,0);
    glDisable(GL_TEXTURE_CUBE_MAP);
}


/**
  Run a gaussian blur on the texture stored in fbo 2 and
  put the result in fbo 1.  The blur should have a radius of 2.

  @param width: the viewport width
  @param height: the viewport height
**/
void GLWidget::renderBlur(int width, int height)
{
    int radius = 2;
    int dim = radius * 2 + 1;
    GLfloat kernel[dim * dim];
    GLfloat offsets[dim * dim * 2];
    createBlurKernel(radius, width, height, &kernel[0], &offsets[0]);
    // TODO: Finish filling this in

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
        //test_camera->mouseDragged(delta.x, delta.y);
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
    //test_camera->mouseDown(event->x(), event->y());
}

/**
  Called when the mouse wheel is turned.  Zooms the camera in and out.
**/
void GLWidget::wheelEvent(QWheelEvent *event)
{
    if (event->orientation() == Qt::Vertical)
    {
        m_camera->mouseWheel(event->delta());
        //test_camera->mouseScrolled(event->delta());
    }
}

/**
  Called when the GLWidget is resized.
 **/
void GLWidget::resizeGL(int width, int height)
{
    // Resize the viewport
    glViewport(0, 0, width, height);

    // Reallocate the framebuffers with the new window dimensions
    foreach (QGLFramebufferObject *fbo, m_framebufferObjects)
    {
        const QString &key = m_framebufferObjects.key(fbo);
        QGLFramebufferObjectFormat format = fbo->format();
        delete fbo;
        m_framebufferObjects[key] = new QGLFramebufferObject(width, height, format);
    }
}

/**
  Draws a textured quad. The texture most be bound and unbound
  before and after calling this method - this method assumes that the texture
  has been bound before hand.

  @param w: the width of the quad to draw
  @param h: the height of the quad to draw
  @param flip: flip the texture vertically
**/
void GLWidget::renderTexturedQuad(int width, int height, bool flip) {
    // Clamp value to edge of texture when texture index is out of bounds
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, flip ? 1.0f : 0.0f);
    glVertex2f(-width/2.0, -height/2.0);

    glTexCoord2f(1.0f, flip ? 1.0f : 0.0f);
    glVertex2f(width/2.0, -height/2.0);

    glTexCoord2f(1.0f, flip ? 0.0f : 1.0f);
    glVertex2f(width/2.0, height/2.0);

    glTexCoord2f(0.0f, flip ? 0.0f : 1.0f);
    glVertex2f(-width/2.0, height/2.0);
    glEnd();
}

void GLWidget::render3DTexturedQuad(int width, int height, bool flip, int depth) {
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, flip ? 1.0f : 0.0f);
    glVertex3f(-width/2.0, -height/2.0, depth);

    glTexCoord2f(1.0f, flip ? 1.0f : 0.0f);
    glVertex3f(width/2.0, -height/2.0, depth);

    glTexCoord2f(1.0f, flip ? 0.0f : 1.0f);
    glVertex3f(width/2.0, height/2.0, depth);

    glTexCoord2f(0.0f, flip ? 0.0f : 1.0f);
    glVertex3f(-width/2.0, height/2.0, depth);
    glEnd();
}

void GLWidget::renderColoredQuad(int width, int height, bool flip) {
    //if glShadeModel is GL_FLAT, it will color the whole quad with the
    //color of the last specified vertex
    glShadeModel(GL_SMOOTH);

    // Draw the  quad
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f(-width/2.0, -height/2.0);

    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(width/2.0, -height/2.0);

    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(width/2.0, height/2.0);

    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-width/2.0, height/2.0);

    //not resetting the color gives weird results
    glColor3f(1.0, 1.0, 1.0);

    glEnd();
}

/**
  Creates a gaussian blur kernel with the specified radius.  The kernel values
  and offsets are stored.

  @param radius: The radius of the kernel to create.
  @param width: The width of the image.
  @param height: The height of the image.
  @param kernel: The array to write the kernel values to.
  @param offsets: The array to write the offset values to.
**/
void GLWidget::createBlurKernel(int radius, int width, int height,
                                                    GLfloat* kernel, GLfloat* offsets)
{
    int size = radius * 2 + 1;
    float sigma = radius / 3.0f;
    float twoSigmaSigma = 2.0f * sigma * sigma;
    float rootSigma = sqrt(twoSigmaSigma * M_PI);
    float total = 0.0f;
    float xOff = 1.0f / width, yOff = 1.0f / height;
    int offsetIndex = 0;
    for (int y = -radius, idx = 0; y <= radius; ++y)
    {
        for (int x = -radius; x <= radius; ++x,++idx)
        {
            float d = x * x + y * y;
            kernel[idx] = exp(-d / twoSigmaSigma) / rootSigma;
            total += kernel[idx];
            offsets[offsetIndex++] = x * xOff;
            offsets[offsetIndex++] = y * yOff;
        }
    }
    for (int i = 0; i < size * size; ++i)
    {
        kernel[i] /= total;
    }
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
    V3 dir(-V3::fromAngles(m_camera->theta, m_camera->phi));
    V3 eye(m_camera->center - dir * m_camera->zoom);
    renderText(10, 50, "eye location : (" + QString::number((double)(eye.x)) +
               ", " + QString::number((double)(eye.y)) + ", " +
               QString::number((double)(eye.x)) + ")", m_font);
}
