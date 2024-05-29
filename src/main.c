#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "state.h"

struct State state;

void
init()
{
        state.window = &window;
        renderer_init( &state.renderer );
}

int
main( int argc, char* argv[] )
{
        window_create( init );
        window_loop();

        return 0;
}
