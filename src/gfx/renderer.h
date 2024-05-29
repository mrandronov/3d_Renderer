#ifndef RENDERER_H
#define RENDERER_H

#define SHADER_COUNT 10

#include <cglm/cglm.h>
#include "../util/camera.h"
#include "voxel.h"

struct Texture
{
        unsigned int    ID;
        int             width;
        int             height;
        int             nrChannels;
        unsigned char*  data;
};

struct Renderer
{
        mat4            model; 
        mat4            view;
        mat4            proj;

        struct Camera   camera;

        struct Shader   shaders[ SHADER_COUNT ];
        struct Shader   current_shader;

        unsigned int    VBOs[2];
        unsigned int    VAOs[2];
        unsigned int    EBO;

        struct Texture  texture;

        struct Voxel    voxels[ 1024 ];
        unsigned int    voxel_count;
};

void    renderer_init( struct Renderer *self );
void    renderer_load_shaders( struct Renderer* self );
void    view_update( struct Renderer* self );
void    renderer_update( struct Renderer* self );
void    renderer_destroy( struct Renderer *self );

#endif /* RENDERER_H */

