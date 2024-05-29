#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <cglm/cglm.h>

#include "../state.h"
#include "window.h"
#include "renderer.h"
#include "../util/camera.h"

/*  Global Window */
struct Window   window;

const float     fps_limit               = 1.0f / 144.0f;
float           last_update_time        = 0.0f;
float           last_frame_time         = 0.0f;
float           delta_time              = 0.0f;

bool            toggle                  = true;
float           camera_speed            = 0.2f;
double          mouse_last_x            = 0.0f;
double          mouse_last_y            = 0.0f;
bool            first_mouse             = true;

static void
error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void
mouse_callback( GLFWwindow* window, double xpos, double ypos )
{
        if ( first_mouse )
        {
                mouse_last_x = xpos;
                mouse_last_y = ypos;
                first_mouse = false;
        }
        float   x_offset = xpos - mouse_last_x;
        float   y_offset = mouse_last_y - ypos;

        mouse_last_x = xpos;
        mouse_last_y = ypos;

        float   sensitivity = 0.1f;
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        state.renderer.camera.yaw += x_offset;
        state.renderer.camera.pitch += y_offset;

        camera_update_direction( &state.renderer.camera );
}

void
process_input()
{
        // Camera Controls
        if ( glfwGetKey( window.handle, GLFW_KEY_W ) == GLFW_PRESS )
        {
                camera_update( &state.renderer.camera, FORWARD, camera_speed );
        }
        if ( glfwGetKey( window.handle, GLFW_KEY_S ) == GLFW_PRESS )
        {
                camera_update( &state.renderer.camera, BACKWARD, camera_speed );
        }
        if ( glfwGetKey( window.handle, GLFW_KEY_SPACE ) == GLFW_PRESS )
        {
                camera_update( &state.renderer.camera, UPWARD, camera_speed );
        }
        if ( glfwGetKey( window.handle, GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
        {
                camera_update( &state.renderer.camera, DOWNWARD, camera_speed );
        }
        if ( glfwGetKey( window.handle, GLFW_KEY_A ) == GLFW_PRESS )
        {
                camera_update( &state.renderer.camera, STRAFE_LEFT, camera_speed );
        }
        if ( glfwGetKey( window.handle, GLFW_KEY_D ) == GLFW_PRESS )
        {
                camera_update( &state.renderer.camera, STRAFE_RIGHT, camera_speed );
        }
}

static void
key_callback( GLFWwindow* window, int key, int scancode, int action, int mods )
{
        if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
                glfwSetWindowShouldClose( window, GLFW_TRUE );
        if ( key == GLFW_KEY_T && action == GLFW_PRESS )
        {
                if ( toggle )
                {
                        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        toggle = false;
                }
                else
                {
                        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                        toggle = true;
                }
        }
        if ( key == GLFW_KEY_L && ( action == GLFW_PRESS || action == GLFW_REPEAT ) )
        {
                state.window->h_offset += 0.08f;
        }
        if ( key == GLFW_KEY_K && ( action == GLFW_PRESS || action == GLFW_REPEAT ) )
        {
                state.window->h_offset -= 0.08f;
        }
}

void
window_create( void (*init)() )
{
        glfwSetErrorCallback( error_callback );

        if ( !glfwInit() )
        {
                printf ("GLFW initialization failed!\n" );
                exit( 1 );
        }

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        window.handle = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "My App", NULL, NULL );
        if ( !window.handle )
        {
                printf( "Window or OpenGL context creation failed!\n" );
                exit(1);
        }

        glfwMakeContextCurrent( window.handle );

        if ( !gladLoadGLLoader( ( GLADloadproc ) glfwGetProcAddress ) )
        {
                printf( "Failed to initialize GLAD!\n" );
                exit( 1 );
        }

        glfwSetKeyCallback( window.handle, key_callback );
        glfwSetCursorPosCallback( window.handle, mouse_callback );
        glfwSetInputMode( window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

        glfwGetFramebufferSize( window.handle, &window.width, &window.height );
        glViewport( 0, 0, window.width, window.height );

        glfwSwapInterval( 1 );

        glEnable( GL_DEPTH_TEST );

        window.init = init;
}

static void
_init()
{
        window.init();
}

static void
_destroy()
{
        renderer_destroy( &state.renderer );

        glfwDestroyWindow( window.handle );
        glfwTerminate();
}

/* glClearColor( 0.13f, 0.13f, 0.13f, 1.0f ); */
/* glClearColor( 0.1921f, 0.2117f, 0.24705f, 1.0f ); */
/* glClearColor( 0.1333f, 0.1568f, 0.19215f, 1.0f ); */

void
window_loop()
{
        _init();

        while ( !glfwWindowShouldClose( window.handle ) )
        {
                float           now = glfwGetTime();
                delta_time = now - last_update_time;

                glfwPollEvents();

                renderer_update( &state.renderer );

                if ( ( now - last_frame_time ) >= fps_limit )
                {
                        process_input();
                        glfwSwapBuffers( window.handle );
                        last_frame_time = now;
                }
                glClearColor( 0.11f, 0.11f, 0.11f, 1.0f );
                glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
                        
                last_update_time = now;
        }

        _destroy();
        exit( 0 );
}

