#ifndef CHUNK_H
#define CHUNK_H

#include "voxel.h"

static const int CHUNK_SIZE = 16;

struct Chunk
{
        vec3            position;
        struct Voxel    voxels[ CHUNK_SIZE ][ CHUNK_SIZE ][ CHUNK_SIZE ];
};

void chunk_create( struct Chunk* self );


#endif /* CHUNK_H */

