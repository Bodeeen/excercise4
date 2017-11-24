
#include "common.hpp"
#include "util.hpp"

using namespace std;
using namespace glm;
using namespace agp;

#define NUM_PARTICLES 100
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

GLuint g_default_vao = 0;
mat4 eye; mat4 M; mat4 V; mat4 P; mat4 MVP;
GLuint program;
vec3 vpos; vec3 lookat;
int paus = 1;
float inertia = 0.5f;

vec3 particles[NUM_PARTICLES]; //position
float sizes[NUM_PARTICLES];
vec3 motion[NUM_PARTICLES];

//memset(particles, 0, NUM_PARTICLES*sizeof(particles));

void init()
{
    int window_width = 1280; int window_height = 800;
    // Generate and bind the default VAO
    glGenVertexArrays(1, &g_default_vao);
    glBindVertexArray(g_default_vao);

    for(int i = 0; i < NUM_PARTICLES; i++)
      sizes[i] = 0.5f + (float)(rand() % 5)/10.0f;

    // Set the background color (RGBA)
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

    // Initialize M and V matrices
    eye = mat4(1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0);
    M = eye;
    vpos = vec3(0,0,50);
    lookat = vec3(0,0,0);
    V = lookAt(
               vpos, // camera position
               lookat, // look at origin
               vec3(0, 1, 0));  // Head is up

    P = perspective(glm::radians(45.0f), (GLfloat) window_width / (GLfloat) window_height, 0.1f, 100.0f);

    MVP = P*V*M;
    // Your OpenGL settings, such as alpha, depth and others, should be
    // defined here! For the assignment, we only ask you to enable the
    // alpha channel.
}


void UpdatePos()
{
  vec3 m;
  for(int i = 0; i < NUM_PARTICLES; i++)
  {
    vec3 motion_upd = vec3(((float)(rand() % 10)-4.5f)/100.0f, ((float)(rand() % 10)-4.5f)/100.0f, ((float)(rand() % 10)-4.5f)/100.0f);
    float scaled_inertia = 0.5f + 0.5*sizes[i]*inertia;
    m = scaled_inertia*motion[i] + (1.0f-scaled_inertia)*motion_upd;
    particles[i] = particles[i] + m;
    motion[i] = m;

    //printf("Particle position = %f, %f, %f \n", particles[i].x, particles[i].y, particles[i].z);
  }
}

void release()
{
    // Release the default VAO
    glDeleteVertexArrays(1, &g_default_vao);

    // Do not forget to release any memory allocation here!
}

void KeyboardCallback(unsigned char key, int x, int y)
{
    switch (key)
    {
      case 27:
        glutLeaveMainLoop();
        break;
      case 49:
        V = scale(V, 1.1f*vec3(1.0f,1.0f,1.0f));
        break;
      case 50:
        V = scale(V, 0.9f*vec3(1.0f,1.0f,1.0f));
        break;
      case 13:
        printf("Updating positions!%s\n");
        UpdatePos();
        break;
      case 112:
        paus = !paus;
        break;
    }
    //printf("Moving\nNew position %f, %f, %f\n", vpos[0], vpos[1], vpos[2] );
    MVP = P*V*M;
}

void SpecialInput(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP:
      V = rotate(V, 0.01f, vec3(1.0f, 0.0f, 0.0f));
      break;
    case GLUT_KEY_DOWN:
      V = rotate(V, -0.01f, vec3(1.0f, 0.0f, 0.0f));
      break;
    case GLUT_KEY_LEFT:
      V = rotate(V, 0.01f, vec3(0.0f, 1.0f, 0.0f));
      break;
    case GLUT_KEY_RIGHT:
      V = rotate(V, -0.01f, vec3(0.0f, 1.0f, 0.0f));
      break;
  }
  //printf("Moving\nNew position %f, %f, %f\n", vpos[0], vpos[1], vpos[2] );
  MVP = P*V*M;
}
void MouseFunc(int button, int state, int x, int y)
{
  switch(button)
  {
    case 3:
      inertia = MIN(1.0f, inertia + 0.05f);
      break;
    case 4:
      inertia = MAX(0.001f, inertia - 0.05f);
      break;
  }
}
void ReshapeWindow(int window_width, int window_height)
{
  glViewport(0, 0, window_width, window_height);

  P = perspective(glm::radians(45.0f), (GLfloat) window_width / (GLfloat) window_height, 0.1f, 100.0f);

  MVP = P*V*M;
}

void display()
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(paus)
      UpdatePos();
    // Your rendering code must be here! Do not forget to swap the
    // front and back buffers, and to force a redisplay to keep the
    // render loop running. This functionality is available within
    // FreeGLUT as well, check the assignment for more information.
    GLuint UniLoc = glGetUniformLocation(program, "MVP");
    for(int i = 0; i < NUM_PARTICLES; i++)
    {
      M = translate(eye, particles[i]);
      MVP = P*V*M;
      glUniformMatrix4fv(UniLoc, 1, GL_FALSE, value_ptr(MVP));
      glutWireSphere(sizes[i], 30, 30);
      glutSolidSphere(sizes[i], 30, 30);
    }
    // Important note: The following function flushes the rendering
    // queue, but this is only for single-buffered rendering. You
    // must replace this function following the previous indications.

    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    // Initialize FreeGLUT and create the window
    glutInit(&argc, argv);

    // Setup the window (e.g., size, display mode and so on)
    glutInitWindowSize(1280, 800);
    //glutInitWindowPosition( ... );
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    // Make FreeGLUT return from the main rendering loop
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,
                  GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    // Create the window and associate the callbacksglutKeyboardFunc(KeyboardCallback);
    glutCreateWindow("Applied GPU Programming");
    glutDisplayFunc(display);
    // glutIdleFunc( ... );
    glutReshapeFunc(ReshapeWindow);
    glutKeyboardFunc(KeyboardCallback);
    glutSpecialFunc(SpecialInput);
    glutMouseFunc(MouseFunc);
    // glutMotionFunc( ... );

    // Init GLAD to be able to access the OpenGL API
    if (!gladLoadGL())
    {
        return GL_INVALID_OPERATION;
    }

    // Display OpenGL information
    util::displayOpenGLInfo();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Initialize the 3D view
    init();

    program = util::loadShaders("vert_shader.glsl", "frag_shader.glsl");
    glUseProgram(program);

    // Launch the main loop for rendering
    glutMainLoop();

    // Release all the allocated memory
    release();

	return 0;
}
