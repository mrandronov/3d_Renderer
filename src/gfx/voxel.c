#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <cglm/cglm.h>

#include "voxel.h"

void
voxel_create( struct Voxel* self, vec3 position )
{
        self->is_active = true;
        glm_vec3( position, self->position );
}

