#ifndef VOXEL_H
#define VOXEL_H

#include <cglm/cglm.h>

static const float              staticVertices[] = {
        -1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,     // 0
         1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 1.0f,       1.0f, 0.0f,     // 1
        -1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,       0.0f, 0.0f,     // 2
         1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 1.0f,       0.0f, 1.0f,     // 3

        -1.0f, -1.0f, -1.0f,    1.0f, 1.0f, 1.0f,       1.0f, 1.0f,     // 4
         1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 1.0f,       1.0f, 0.0f,     // 5
        -1.0f,  1.0f, -1.0f,    0.0f, 0.0f, 1.0f,       0.0f, 0.0f,     // 6
         1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 1.0f,       0.0f, 1.0f,     // 7
};

static const unsigned int       staticIndices[] = {
        //Top
        2, 6, 7,
        2, 3, 7,
        
        //Bottom
        0, 4, 5,
        0, 1, 5,
        
        //Left
        0, 2, 6,
        0, 4, 6,
        
        //Right
        1, 3, 7,
        1, 5, 7,
        
        //Front
        0, 2, 3,
        0, 1, 3,
        
        //Back
        4, 6, 7,
        4, 5, 7
};

struct Voxel
{
        unsigned int    is_active;
        vec3            position;       // Voxel position relative to world space
};

void    voxel_create( struct Voxel* self, vec3 position );

#endif /* VOXEL_H */
