#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

enum Camera_Movement
{
        FORWARD,
        BACKWARD,
        UPWARD,
        DOWNWARD,
        STRAFE_LEFT,
        STRAFE_RIGHT
};

struct Camera
{
        vec3            pos;
        vec3            front;
        vec3            up;
        vec3            direction;

        float           yaw;
        float           pitch;
};

void camera_create( struct Camera* self );

void camera_update( struct Camera* self, enum Camera_Movement move, float camera_speed );

void camera_update_direction( struct Camera* self );

#endif /* CAMERA_H */

