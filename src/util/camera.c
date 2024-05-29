#include <math.h>

#include <cglm/cglm.h>
#include "camera.h"

void
camera_create( struct Camera* self )
{
        *self = ( struct Camera ) { 0 };

        glm_vec3_zero( self->pos );
        self->pos[ 2 ] = 3.0f;

        glm_vec3_zero( self->front );
        /* self->front[ 2 ] = -1.0f; */
        
        glm_vec3_zero( self->up );
        self->up[ 1 ] = 1.0f;

        self->yaw = 0.0f;
        self->pitch = 0.0f;

        camera_update_direction( self );
}

void
camera_update( struct Camera* self, enum Camera_Movement move, float camera_speed )
{
        if ( move == FORWARD )
        {
                vec3 temp;
                glm_vec3_scale( self->front, camera_speed, temp );
                glm_vec3_add( self->pos, temp, self->pos );
        }
        if ( move == BACKWARD )
        {
                vec3 temp;
                glm_vec3_scale( self->front, camera_speed, temp );
                glm_vec3_sub( self->pos, temp, self->pos );
        }
        if ( move == UPWARD )
        {
                vec3 temp;
                glm_vec3_scale( self->up, camera_speed, temp );
                glm_vec3_add( self->pos, temp, self->pos );
        }
        if ( move == DOWNWARD )
        {
                vec3 temp;
                glm_vec3_scale( self->up, camera_speed, temp );
                glm_vec3_sub( self->pos, temp, self->pos );
        }
        if ( move == STRAFE_LEFT )
        {
                vec3 temp;
                glm_vec3_cross( self->front, self->up, temp );
                glm_vec3_normalize(temp);
                glm_vec3_scale( temp, camera_speed, temp );
                glm_vec3_sub( self->pos, temp, self->pos );
        }
        if ( move == STRAFE_RIGHT )
        {
                vec3 temp;
                glm_vec3_cross( self->front, self->up, temp );
                glm_vec3_normalize(temp);
                glm_vec3_scale( temp, camera_speed, temp );
                glm_vec3_add( self->pos, temp, self->pos );
        }

}

void camera_update_direction( struct Camera* self )
{
        if ( self->pitch > 89.0f )
        {
                self->pitch = 89.0f;
        }
        if ( self->pitch < -89.0f )
        {
                self->pitch = -89.0f;
        }

        self->direction[ 0 ] = cos( glm_rad( self->yaw ) ) * cos( glm_rad( self->pitch ) );
        self->direction[ 1 ] = sin( glm_rad( self->pitch ) );
        self->direction[ 2 ] = sin( glm_rad( self->yaw ) ) * cos( glm_rad( self->pitch ) );

        glm_vec3_normalize_to( self->direction, self->front );
}


