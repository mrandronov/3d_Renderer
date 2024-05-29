#ifndef WINDOW_H
#define WINDOW_H

#define WINDOW_WIDTH 1920 
#define WINDOW_HEIGHT 1080 

struct Window
{
        int             width;
        int             height;
        float           h_offset;
        GLFWwindow*     handle;
        void            (*init)();
};

void                    window_create( void (*init)() );
void                    window_loop();

extern struct Window    window;

#endif /* WINDOW_H */
