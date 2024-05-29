#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>

struct Shader
{
        unsigned int    ID;
};

struct Shader   shader_create( char* vertex_path, char* fragment_path );

void            shader_use( struct Shader* self );

void            shader_setBool( struct Shader* self, char* name, bool value );
void            shader_setInt( struct Shader* self, char* name, int value );
void            shader_setFloat( struct Shader* self, char* name, float value );

void            shader_destroy( struct Shader* self );

#endif /* SHADER_H */
