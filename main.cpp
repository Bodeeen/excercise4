
#include "common.hpp"
#include "util.hpp"

using namespace std;
using namespace glm;
using namespace agp;

GLuint g_default_vao = 0;
mat4 M; mat4 V; mat4 P; mat4 MVP;
GLuint program;
vec3 vpos; vec3 lookat;

void init()
{
    int window_width = 1280; int window_height = 800;
    // Generate and bind the default VAO
    glGenVertexArrays(1, &g_default_vao);
    glBindVertexArray(g_default_vao);

    // Set the background color (RGBA)
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

    // Initialize M and V matrices
    M = mat4(1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 0.0, 1.0);

    vpos = vec3(0,0,1);
    lookat = vec3(0,0,0);
    V = lookAt(
               vpos, // camera position
               lookat, // look at origin
               vec3(0, 1, 0));  // Head is up

    P = perspective(45.0f, (GLfloat) window_width / (GLfloat) window_height, 0.1f, 10.0f);

    MVP = P*V*M;
    // Your OpenGL settings, such as alpha, depth and others, should be
    // defined here! For the assignment, we only ask you to enable the
    // alpha channel.
}

void release()
{
    // Release the default VAO
    glDeleteVertexArrays(1, &g_default_vao);

    // Do not forget to release any memory allocation here!
}

void KeyboardCallback(unsigned char key, int x, int y)
{
    printf("Key pressed: %d\n", key);
    switch (key)
    {
      case 27:
        glutLeaveMainLoop();
        break;
      case 49:
        vpos = vec3(vpos.x, vpos.y, vpos.z+0.05f);
        break;
      case 50:
        vpos = vec3(vpos.x, vpos.y, vpos.z-0.05f);
        break;
    }
    printf("Moving\nNew position %f, %f, %f\n", vpos[0], vpos[1], vpos[2] );
    V = lookAt(
               vpos, // camera position
               lookat, // look at origin
               vec3(0, 1, 0));  // Head is up
    MVP = P*V*M;
}


void display()
{
    // Clear the screen
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Your rendering code must be here! Do not forget to swap the
    // front and back buffers, and to force a redisplay to keep the
    // render loop running. This functionality is available within
    // FreeGLUT as well, check the assignment for more information.
    glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, value_ptr(MVP));
    // Important note: The following function flushes the rendering
    // queue, but this is only for single-buffered rendering. You
    // must replace this function following the previous indications.

    glutSolidSphere(0.2f, 100 , 100);
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
    // glutReshapeFunc( ... );
    glutKeyboardFunc(KeyboardCallback);
    // glutSpecialFunc( ... );
    // glutMouseFunc( ... );
    // glutMotionFunc( ... );

    // Init GLAD to be able to access the OpenGL API
    if (!gladLoadGL())
    {
        return GL_INVALID_OPERATION;
    }

    // Display OpenGL information
    util::displayOpenGLInfo();

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
