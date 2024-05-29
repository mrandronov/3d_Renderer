#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "../state.h"
#include "shader.h"
#include "renderer.h"

void
load_voxel( struct Renderer* self )
{
        glBindVertexArray( self->VAOs[ 0 ] );
        
        glBindBuffer( GL_ARRAY_BUFFER, self->VBOs[ 0 ] );
        glBufferData( GL_ARRAY_BUFFER, sizeof( staticVertices ), staticVertices, GL_STATIC_DRAW );

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, self->EBO );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( staticIndices ), staticIndices, GL_STATIC_DRAW );

        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) 0 );
        glEnableVertexAttribArray( 0 );

        glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 3 * sizeof( float ) ) );
        glEnableVertexAttribArray( 1 );

        glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof( float ), ( void* ) ( 6 * sizeof( float ) ) );
        glEnableVertexAttribArray( 2 );
}

void
draw_voxel( struct Renderer* self, struct Voxel* voxel )
{
        mat4 temp;
        glm_mat4_copy(self->model, temp );

        glm_translate( self->model, voxel->position );
        unsigned int modelLoc = glGetUniformLocation( self->current_shader.ID, "model" );
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, ( const GLfloat* ) &self->model );

        glm_mat4_copy( temp, self->model );

        unsigned int viewLoc = glGetUniformLocation( self->current_shader.ID, "view" );
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, ( const GLfloat* ) &self->view );

        unsigned int projLoc = glGetUniformLocation( self->current_shader.ID, "projection" );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, ( const GLfloat* ) &self->proj );

        shader_setFloat( &self->current_shader, "horizontal_offset", state.window->h_offset );
        shader_use( &self->current_shader );
        
        glBindTexture( GL_TEXTURE_2D, self->texture.ID );
        glBindVertexArray( self->VAOs[ 0 ] );
        glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0 );
        glBindVertexArray( 0 );
}

void
renderer_load_shaders( struct Renderer *self )
{
        glGenBuffers( 1, self->VBOs );
        glGenVertexArrays( 1, self->VAOs );
        glGenBuffers( 1, &self->EBO );

        load_voxel( self );

        self->shaders[ 0 ] = shader_create( "./res/shaders/vertex_shader.vs",
                                                "./res/shaders/fragment_shader.fs" );
        self->shaders[ 1 ] = shader_create( "./res/shaders/vertex_shader.vs",
                                                "./res/shaders/second_color.fs" );

        self->current_shader = self->shaders[ 0 ];
}

void
view_update( struct Renderer* self )
{
        // Update the camera here for all objects
        vec3 camera_center;
        glm_vec3_add( self->camera.pos, self->camera.front, camera_center );
        glm_lookat( self->camera.pos, camera_center, self->camera.up, self->view );
}

void
renderer_update( struct Renderer* self )
{
        for ( int i = 0; i < self->voxel_count; i++ )
        {
                draw_voxel( self, &self->voxels[ i ] );
        }
        view_update( self );
}

void
add_voxel( struct Renderer* self, vec3 position )
{
        voxel_create( &self->voxels[ self->voxel_count ], position );
        self->voxel_count++;
}

void
renderer_init( struct Renderer* self )
{
        *self = ( struct Renderer ) { 0 };


        /* TEXTURE LOADING */
        self->texture.data = stbi_load( "./res/cube.jpg",
                        &self->texture.width,
                        &self->texture.height,
                        &self->texture.nrChannels,
                        0 );
        if ( !self->texture.data )
        {
                printf( "Failed to load texture!\n" );
                exit( 1 );
        }

        glGenTextures( 1, &self->texture.ID );
        glBindTexture( GL_TEXTURE_2D, self->texture.ID );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, self->texture.width, self->texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, self->texture.data );
        glGenerateMipmap( GL_TEXTURE_2D );

        stbi_image_free( self->texture.data );

        // set the texture wrapping/filtering options (on the currently bound texture object)
        /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	 */
        /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); */
        /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); */
        /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); */

        /* END TEXTURE LOADING */

        self->voxel_count = 0;

        float spacer = 6.0f;
        for ( int i = 0; i < 8; i++ )
        {
                for ( int j = 0; j < 8; j++ )
                {
                        for ( int k = 0; k < 8; k++ )
                        {
                                add_voxel( self, (vec3) { ( float ) i * spacer, ( float ) k * spacer, ( float ) j * spacer } );
                        }
                }
        }

        camera_create( &self->camera );

        glm_mat4_identity( self->model );
        /* glm_translate( self->model, ( vec3 ){ 0.0f, 0.0f, 1.0f } ); */

        glm_mat4_identity( self->view );
        glm_translate( self->view, ( vec3 ){ 0.0f, 0.0f, -3.0f } );

        vec3 camera_center;
        glm_vec3_add( self->camera.pos, self->camera.front, camera_center );
        glm_lookat( self->camera.pos, camera_center, self->camera.up, self->view );

        glm_perspective( glm_rad( 60.0f ), 
                        ( float ) window.width / ( float ) window.height, 
                        0.1f, 
                        100.0f, 
                        self->proj );

        renderer_load_shaders( self );
}


void
renderer_destroy( struct Renderer* self )
{
        glDeleteVertexArrays( 1, self->VAOs );
        
        glDeleteBuffers( 1, self->VBOs );
        glDeleteBuffers( 1, &self->EBO );
}

