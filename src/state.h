#ifndef STATE_H
#define STATE_H

#include "gfx/shader.h"
#include "gfx/renderer.h"
#include "gfx/window.h"

struct State
{
        struct Window*          window;
        struct Renderer         renderer;
};

extern struct State state;

#endif /* STATE_H */
