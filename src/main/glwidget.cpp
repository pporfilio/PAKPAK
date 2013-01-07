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
#include <GLU.h>
#include "glm.h"
#include "utils.h"
#include <QElapsedTimer>
#include "plane3d.h"

#include <sstream>


using std::cout;
using std::endl;

extern "C"
{
    extern void /*APIENTRY*/ glActiveTexture(GLenum);
}

static const int MAX_FPS = 120;
static const int N_FRAME_TIMES = 20;
static const int SUBDIVS = 4;

/**
  Constructor.  Initialize all member variables here.
 **/
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent),
    m_timer(this), m_prevTime(0), m_prevFps(0.f), m_fps(0.f),
    m_font("Deja Vu Sans Mono", 16, 4)
{
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);

    m_gameCamera = new GameCamera();
    m_orbitCamera = new OrbitCamera();
    //m_camera = m_orbitCamera;
    m_camera = m_gameCamera;

    F_Z3 = 0.0;
    F_C = Vector4(-.1, .1, .5, -.6);
    F_specular = true;
    F_reflect = true;

    F_reflect_channels = Vector3(1.0, 1.0, 1.0);
    julia_selected = false;
    mandelbox_selected = true;
    skybox_enabled = false;

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

    m_paused = false;
    //m_paused = true;
    m_show_text = true;

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(update()));

    /* hack because I don't know the right way to do this in C/C++ */

    string path1 = "../src/shaders/fractal.frag";
    string path2 = "../../../../src/shaders/fractal.frag";
    FILE* f1 = fopen(path1.c_str(), "r");
    FILE* f2 = fopen(path2.c_str(), "r");

    if (f1) {
        m_base_path = new string("../");
        m_resources_path = new string("../resources/");
        fclose(f1);
    } else if (f2) {
        m_base_path = new string("../../../../");
        m_resources_path = new string("../../../../resources/");
    } else {
        printf("cannot find fractal fragment shader!\n");
    }

    if (f2) {
        fclose(f2);
    }

    frameNumber = 0;
    isRecording = false;

    m_frame_times = new int[N_FRAME_TIMES];
    for (int i = 0; i < N_FRAME_TIMES; i++) {
        m_frame_times[i] = 0;
    }
    m_curr_time_pos = 0;
    m_prevTime = m_clock.elapsed();

    m_newView = true;
    m_counter = 0;
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
    delete m_frame_times;
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

    createFramebufferObjects(this->width(), this->height());
    cout << "Created Framebuffer Objects..." << endl;

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

    tmp1 = "";
    tmp2 = "";
    tmp1 += shader_base;
    tmp2 += shader_base;
    m_shaderPrograms["mandelboxregion"] = ResourceLoader::newShaderProgram(ctx, tmp1.append("mandelboxregion.vert").c_str(),
                                                                        tmp2.append("mandelboxregion.frag").c_str());

}


void GLWidget::createFramebufferObjects(int width, int height)
{

    // Allocate the main framebuffer object for rendering the scene to
    // This needs a depth attachment
    m_framebufferObjects["fbo_0"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,//QGLFramebufferObject::Depth,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    //m_framebufferObjects["fbo_0"]->format().setSamples(16);
    // Allocate the secondary framebuffer obejcts for rendering textures to (post process effects)
    // These do not require depth attachments
    m_framebufferObjects["fbo_1"] = new QGLFramebufferObject(width, height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);
    // TODO: Create another framebuffer here.  Look up two lines to see how to do this... =.=
    m_mb_width = width/SUBDIVS;
    m_mb_height = height/SUBDIVS;
    m_framebufferObjects["minibuffer"] = new QGLFramebufferObject(m_mb_width, m_mb_height, QGLFramebufferObject::NoAttachment,
                                                             GL_TEXTURE_2D, GL_RGB16F_ARB);

    m_framebufferObjects["test"] = new QGLFramebufferObject(width * 2, height * 2, QGLFramebufferObject::NoAttachment,//QGLFramebufferObject::Depth,
                                                            GL_TEXTURE_2D, GL_RGB16F_ARB);

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

void GLWidget::paintMandelboxHelper(char *srcBuffer, sub_region_t *sub_region, Plane3D *full_plane,
                                    int target_width, int target_height) {

    glViewport(0, 0, m_mb_width, m_mb_height);
    //bind the fbo to render to, and pass the subregion in world
    //space to the rendering shader
    m_framebufferObjects[srcBuffer]->bind();
    renderMandelboxRegion(sub_region->plane, full_plane);
    m_framebufferObjects[srcBuffer]->release();
    //restore viewport to copy between fbos and render to the screen.
    glViewport(0, 0, this->width(), this->height());

    //copy the minibuffer to the region it corresponds to in the full buffer
    m_framebufferObjects[srcBuffer]->blitFramebuffer(m_framebufferObjects["fbo_1"],
                                               QRect(sub_region->col * m_mb_width,
                                                     sub_region->row * m_mb_height,
                                                     target_width, target_height),
                                               m_framebufferObjects[srcBuffer],
                                               QRect(0, 0, m_mb_width, m_mb_height),
                                               GL_COLOR_BUFFER_BIT,
                                               GL_NEAREST);
}

/**
  Draws the scene to a buffer which is rendered to the screen when this function exits.
 **/
void GLWidget::paintGL()
{

    fflush(stdout); //just to make sure things print at least once per frame

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

    //every other frame has 17 ms run...add only those frames where shader actually runs.
    //also solves issue with paused, etc.
    if (time - m_prevTime > 100) {
        m_frame_times[m_curr_time_pos] = (time - m_prevTime);
        m_curr_time_pos ++;
        m_curr_time_pos %= N_FRAME_TIMES;

        int sum = 0;
        int count = 0;
        for (int i = 0; i < N_FRAME_TIMES; i++) {
            if (m_frame_times[i] > 0) {
                sum += m_frame_times[i];
                count ++;
            }
        }
//        printf("single frame time = %d\n", time - m_prevTime);
//        printf("avg frame time = %.4f\n", (float)sum / count);
    }

    m_prevTime = time;

    if (m_paused) {
        paintText();
        usleep(1000);
        return;
    }


    Matrix4x4 film_to_world = m_camera->getFilmToWorld(this->width(), this->height());

    float plane_depth = 2.0;

    float aspect = (float)this->width()/(float)this->height();

    float half_height = plane_depth * tan(toRadians(m_camera->getFOVY()/2));
    float half_width = half_height * aspect;

    //For each frame we compute a plane that is the "film" of the camera. That is,
    //it is perpendicular to the look vector and sits a fixed distance (plane_depth)
    //in front of the camera position. We then raycast from the camera through
    //different points on this plane.
    Plane3D *plane = new Plane3D(-half_width, half_width, half_height, -half_height, plane_depth);

    //to do multi-stage rendering, we subdivide the plane into SUBDIVS x SUBDIVS
    //equally-sized regions and render them one at a time.
    sub_region_t *subdivPlanes = plane->subdivide(SUBDIVS);


    //Since the openGL camera is always at the origin and we treat camera moves
    //as transformations of the objects, these planes are always created in the
    //same location, with only the aspect dependent on the camera. Then, we
    //transform the main plane and each sub-region according to the camera's
    //transformation matrix. Doing this transformation after the subdivision
    //makes it easier to do the subdivision.
    plane->transform(film_to_world);

    for (int i = 0; i < SUBDIVS * SUBDIVS; i++) {
        subdivPlanes[i].plane->transform(film_to_world);
    }

    //set the screen size parameters which will be used in non-region-based rendering
    t_ur = plane->m_ur;
    t_ul = plane->m_ul;
    t_lr = plane->m_lr;
    t_ll = plane->m_ll;

    //used for supersampling
    half_pixel_size = .5*(t_ur - t_ul).getMagnitude()/float(this->width());

    //set up the camera
    applyPerspectiveCamera(this->width(), this->height());

    //// Render Sky Box
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable cube maps and draw the skybox


    if (mandelbox_selected) {
        skybox_enabled = false;
    }

    if (skybox_enabled) {
        glEnable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMap);
        glCallList(m_skybox);
    } else {
        glDisable(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    if (julia_selected) {
        renderFractal();
    } else if (mandelbox_selected) {

        //having this enabled makes each frame layer on top of the previous
        glDisable(GL_BLEND);

        char *src = "minibuffer";

//        for (int i = 0; i < SUBDIVS * SUBDIVS; i++) {

//        }

        if (m_newView) {
            //for our initial render, set up the region as the whole plane.
            //this way, we'll render at the lower resolution of the minibuffer
            //but spread accross the whole view, thus giving a very fast and
            //very low-res initial image.
            sub_region_t full_plane_region;
            full_plane_region.plane = plane;
            full_plane_region.row = 0;
            full_plane_region.col = 0;
            m_newView = false;
            m_counter = 0;

            paintMandelboxHelper(src, &full_plane_region, plane, this->width(), this->height());
        } else {
            //if counter is greater than this, then we've filled all the subregions
            if (m_counter < SUBDIVS * SUBDIVS) {
                paintMandelboxHelper(src, &subdivPlanes[m_counter], plane, m_mb_width, m_mb_height);
                m_counter ++;
            }
        }

        //Now that we have the image rendered into a framebuffer, switch to
        //an orthogonal camera so we can just paste the image onto the screen.
        applyOrthogonalCamera(this->width(), this->height());

        //use what we have in framebuffer 1 as a texture for this ortho quad
        glBindTexture(GL_TEXTURE_2D, m_framebufferObjects["fbo_1"]->texture());

        //render the quad.
        renderTexturedQuad(this->width(), this->height(), false);
        glBindTexture(GL_TEXTURE_2D, 0);

    }

    glDisable(GL_DEPTH_TEST);

    if (skybox_enabled) {
        glBindTexture(GL_TEXTURE_CUBE_MAP,0);
        glDisable(GL_TEXTURE_CUBE_MAP);
    }

    paintText();

//    printf("rendered frame...\n");
    if (isRecording) {
        printf("calling savePicture...\n");
        savePicture();
        frameNumber++;
    }

    delete plane;
    for (int i = 0; i < SUBDIVS * SUBDIVS; i++) {
        delete subdivPlanes[i].plane;
    }
    free(subdivPlanes);
}


void GLWidget::applyOrthogonalCamera(float width, float height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0.f, -1.f, 1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::renderTexturedQuad(int width, int height, bool flip) {
    // Clamp value to edge of texture when texture index is out of bounds
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Draw the  quad
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, flip ? 1.0f : 0.0f);
    glVertex2f(0.0f, 0.0f);
    glTexCoord2f(1.0f, flip ? 1.0f : 0.0f);
    glVertex2f(width, 0.0f);
    glTexCoord2f(1.0f, flip ? 0.0f : 1.0f);
    glVertex2f(width, height);
    glTexCoord2f(0.0f, flip ? 0.0f : 1.0f);
    glVertex2f(0.0f, height);
    glEnd();
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

//    glClearColor(bg_color.x,bg_color.y,bg_color.z, 1);

    render3DTexturedQuad(this->width(), this->height(), true); //AIMEI

    m_shaderPrograms["fractal"]->release();

}

void GLWidget::renderMandelboxRegion(Plane3D *region, Plane3D *fullFilm) {

        glDisable(GL_DEPTH_TEST);

        V3 pos = m_camera->getPos();

        //pass parameters to the shader
        QGLShaderProgram *prog = m_shaderPrograms["mandelboxregion"];
        prog->bind();
        prog->setUniformValue("world_eye", pos.x, pos.y, pos.z);
        prog->setUniformValue("halfPix", half_pixel_size);
        prog->setUniformValue("specular_enabled", F_specular ? 1 : 0);
        prog->setUniformValue("ss_enabled", ss_enabled ? 1 : 0);
        prog->setUniformValue("coloring", mandelbox_coloring);
        prog->setUniformValue("colorScheme", mandelbox_colorScheme);
        prog->setUniformValue("fog_enabled", mandelbox_fog);
        prog->setUniformValue("ITR", mandelbox_itr);
        prog->setUniformValue("EPSILON", mandelbox_epsilon);
        prog->setUniformValue("BREAK", mandelbox_break);
        prog->setUniformValue("DEPTH", mandelbox_depth);

        AABB *ffaabb = fullFilm->getAABB();
        AABB *raabb = region->getAABB();
//        printf("-----------------------------\n");
//        raabb->print();
//        ffaabb->print();
        prog->setUniformValue("fullMinX", ffaabb->getMinX());
        prog->setUniformValue("fullMinY", ffaabb->getMinY());
        prog->setUniformValue("fullMinZ", ffaabb->getMinZ());
        prog->setUniformValue("fullWidth", ffaabb->getWidth());
        prog->setUniformValue("fullHeight", ffaabb->getHeight());
        prog->setUniformValue("fullDepth", ffaabb->getDepth());
        prog->setUniformValue("regionMinX", raabb->getMinX());
        prog->setUniformValue("regionMinY", raabb->getMinY());
        prog->setUniformValue("regionMinZ", raabb->getMinZ());
        prog->setUniformValue("regionWidth", raabb->getWidth());
        prog->setUniformValue("regionHeight", raabb->getHeight());
        prog->setUniformValue("regionDepth", raabb->getDepth());


        render3DTexturedQuad(this->width(), this->height(), true); //AIMEI

        //clean-up
        m_shaderPrograms["mandelbox"]->release();


}


//Set up fragment shader and render it to a quad that fills the screen
void GLWidget::renderMandelbox() {

//    m_framebufferObjects["fbo_0"]->bind();

//    //allows mandelbox to be drawn on top of whatever was previously in the buffer...
//    //or something...didn't work without this.
    glDisable(GL_DEPTH_TEST);

    V3 pos = m_camera->getPos();

    //pass parameters to the shader
    m_shaderPrograms["mandelbox"]->bind();
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



    render3DTexturedQuad(this->width(), this->height(), true); //AIMEI

    //clean-up
    m_shaderPrograms["mandelbox"]->release();
//    m_framebufferObjects["fbo_0"]->release();

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
        m_newView = true;
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
    m_newView = true;
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
    m_newView = true;
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
    m_newView = true;
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

    if (isRecording) {
        qi.save(QFileInfo(recordName).absoluteDir().absolutePath() + "/"
                + QFileInfo(recordName).baseName() + "_"
                + QString::number((double)frameNumber) + ".png", "PNG", 100);
    } else {
        //TODO: put saving a single screenshot somewhere else
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("PNG Image (*.png)"), &filter);
        qi.save(QFileInfo(fileName).absoluteDir().absolutePath() + "/" + QFileInfo(fileName).baseName() + ".png", "PNG", 100);

    }
}

void GLWidget::loadCamLocation() {
    QString path = myGetSaveFileName(this, tr("Camera File Location"),
                                     QString(m_resources_path->c_str()), tr("*"), false);
    char path_str[path.size()]; //here size includes the null terminating character
    QStringToChar(&path, path_str);

    FILE *f = fopen(path_str, "r");
    if (f) {
        CameraState *s = readCameraState(f);
        if (s) {
            //If the current camera is the orbit camera, switch to the game camera
            // TODO: Figure out how to update UI to correctly reflect the selected camera.
            if (m_camera->getType() == ORBIT_CAMERA) {
                m_camera = m_gameCamera;
            }
            //then load the state into the game camera.
            m_camera->setState(s);
        } else {
            printf("reading camera state failed. Not updating camera\n");
        }
        fclose(f);
    } else {
        printf("could not open file for reading at %s\n", path_str);
    }
}


void GLWidget::saveCamLocation() {
    QString path = myGetSaveFileName(this, tr("Camera File Location"),
                                     QString(m_resources_path->c_str()), tr("*"), true);
    char path_str[path.size()];
    QStringToChar(&path, path_str);

    FILE *f = fopen(path_str, "w");
    if (f) {
        writeCameraState(f, m_camera);
        fclose(f);
    } else {
        printf("could not open file for writing at %s\n", path_str);
    }
}


/**
  Draws text for the FPS and screenshot prompt
 **/
void GLWidget::paintText()
{

    if (! m_show_text) {
        return;
    }
    //glColor3f(1.f, 1.f, 1.f);
    glColor3f(0.f, 0.f, 0.f);

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

    renderText(10, 95, "resolution = " + QString::number(this->width()) + " x " + QString::number(this->height()));


    //reset color to white
    glColor3f(1.f, 1.f, 1.f);


}


void GLWidget::savePointCloud() {
    QString path = myGetSaveFileName(this, tr("Camera File Location"),
                                     QString(m_resources_path->c_str()), tr("*"), true);
    char path_str[path.size()];
    QStringToChar(&path, path_str);

    float step = .1;

    FILE *f = fopen(path_str, "w");
    if (f) {
        printf("computing points...\n");
        std::vector<V3> pc = computePointCloud(step);
        printf("writing points...\n");
        writePointCloud(f, pc, step);
        fclose(f);
        printf("point cloud saved\n");
    } else {
        printf("could not open file for writing at %s\n", path_str);
    }

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
        m_newView = true;
    }
}

void GLWidget::radioToggeled_Game_Cam(bool checked) {
    if (checked) {
        m_camera = m_gameCamera;
        m_newView = true;
    }
}

void GLWidget::resetCurrentCamera() {
    m_camera->reset();
    m_newView = true;
}

void GLWidget::sliderUpdate_ITR(int newValue) {
    mandelbox_itr = newValue;
    m_newView = true;
}

void GLWidget::sliderUpdate_EPS(int newValue) {
    mandelbox_epsilon = float(newValue)/10000000.0;
    m_newView = true;
}

void GLWidget::sliderUpdate_BRK(int newValue) {
    mandelbox_break = newValue;
    m_newView = true;
}

void GLWidget::sliderUpdate_DEP(int newValue) {
    mandelbox_depth = newValue;
    m_newView = true;
}

void GLWidget::checkRecord(bool checked) {
    /*  [Parker] 12-22-11
    I added this, then decided I don't actually want that behavior yet
    //reset frame number each time we start recording
    if (!isRecording && checked) {
        frameNumber = 0;
    }
    */
    if (! checked) {
        isRecording = checked;
    }

    if (!isRecording && checked) {
        //stop using GPU while selecting a file.
        recordName = myGetSaveFileName(this, tr("Save Image"),
                                       QString(m_resources_path->c_str()),
                                       tr("PNG Image (*.png)"), true);
        printf("got name\n");

        //if the dialog returned the empty string
        if (recordName.compare("") == 0) {
            printf("user cancelled!\n");
            //TODO: Make record in the UI become unchecked if the user cancels.
            //      I don't know how to do this without putting the file dialog in
            //      mainwindow, which doesn't really make sense because of the pausing behavior.
            isRecording = false;
        } else {
            isRecording = checked;
        }
    }
}

//Prompts user to select a file or directory using QFileDialog, but also
//handles pausing the GL drawing during the interaction so it is faster.
//Will present a save dialog if save is true, else presents an open dialog.
QString GLWidget::myGetSaveFileName(QWidget *parent, const QString &caption,
                                    const QString &dir, const QString &filter,
                                    bool save) {

    bool prior_pause_state = m_paused;
    setPaused(true);

    QString selectedFilter;
    QString selected_path;

    if (save) {
        selected_path = QFileDialog::getSaveFileName(parent, caption,
                                                     dir, filter, &selectedFilter);
    } else {
        selected_path = QFileDialog::getOpenFileName(parent, caption,
                                                     dir, filter, &selectedFilter);
    }

    setPaused(prior_pause_state);
    return selected_path;
}

void GLWidget::setPaused(bool checked) {
    m_paused = checked;
}

void GLWidget::setShowText(bool checked) {
    m_show_text = checked;
}
