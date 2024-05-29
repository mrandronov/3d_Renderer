#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <cglm/cglm.h>

#include "shader.h"

void
read_shader_file( char* shader_path, char** buffer )
{
        FILE*   file = fopen( shader_path, "r" );
        long    file_length;

        if ( !file )
        {
                printf( "Error opening shader from %s!\n", shader_path );
                printf( "Error: %s\n", strerror( errno ) );
                exit( 1 );
        }

        fseek( file, 0, SEEK_END );
        file_length = ftell( file );
        fseek( file, 0, SEEK_SET );

        *buffer = ( char* ) malloc( sizeof( char ) * file_length );
        if ( *buffer )
        {
                if ( fread( *buffer, 1, file_length, file ) == 0 )
                {
                        printf( "Error reading file contents!\n" );
                        exit( 1 );
                }
        }       

        fclose( file );

        printf( "Shader files %s successfully read.\n", shader_path );
}

unsigned int
initialize_shader( char* shader_path, GLenum shader_type )
{
        char*           shaderBuffer = 0;
        read_shader_file( shader_path, &shaderBuffer );

        unsigned int    shader = glCreateShader( shader_type );
        glShaderSource( shader, 1, (const char** )&shaderBuffer, NULL );
        glCompileShader( shader );

        int             success;
        char            infoLog[ 512 ];

        glGetShaderiv( shader, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
                glGetShaderInfoLog( shader, 512, NULL, infoLog );
                printf( "Shader compilation error (%d):\n\t{\t\t%s\t}\n", success, infoLog );
                exit( 1 );
        }

        return shader;
}

struct Shader
shader_create( char* vertex_shader_path, char* fragment_shader_path )
{
        struct Shader self;

        unsigned int    vertex_shader = 0;
        unsigned int    fragment_shader = 0;

        vertex_shader = initialize_shader( vertex_shader_path, GL_VERTEX_SHADER );
        fragment_shader = initialize_shader( fragment_shader_path, GL_FRAGMENT_SHADER );

        unsigned int    shader_program = 0;
        shader_program = glCreateProgram();
        
        glAttachShader( shader_program, vertex_shader );
        glAttachShader( shader_program, fragment_shader );
        glLinkProgram( shader_program );

        int             success;
        char            infoLog[ 512 ];
        
        glGetProgramiv( shader_program, GL_LINK_STATUS, &success );
        if ( !success )
        {
                glGetProgramInfoLog( shader_program, 512, NULL, infoLog );
                printf( "Shader program linking error (%d):\n\t{\n\t\t%s\t}\n", success, infoLog );
                exit( 1 );
        }

        glDeleteShader( vertex_shader );
        glDeleteShader( fragment_shader );

        self.ID = shader_program;

        return self;
}

void
shader_use( struct Shader* self )
{
        glUseProgram( self->ID );
}



void
shader_setBool( struct Shader* self, char* name, bool value )
{
        glUniform1i( glGetUniformLocation( self->ID, name ), ( int ) value );
}

void
shader_setInt( struct Shader* self, char* name, int value )
{
        glUniform1i( glGetUniformLocation( self->ID, name ), value );
}

void
shader_setFloat( struct Shader* self, char* name, float value )
{
        glUniform1f( glGetUniformLocation( self->ID, name ), value );
}


void
shader_destroy( struct Shader* self )
{
        /* glDestroyProgram( shader->id ); */
        /* free( shader ); */
}


