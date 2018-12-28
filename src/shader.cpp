#include "shader.hpp"

/*
#include "matrix.h"

#include <cstdio>
#include <cstring>

static const char vertex_shader[] =
    "attribute vec4 position;\n"
    "attribute vec4 inTexCoord;\n"
    "varying vec2 texCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 modelview;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   gl_Position = modelview * position;\n"
    "   texCoord = inTexCoord.xy;\n"
    "}\n";

static const char fragment_shader[] =
    "varying vec2 texCoord;\n"
    "uniform sampler2D tex;\n"
    "\n"
    "void main()\n"
    "{\n"
    "   // gl_FragColor = vec4( texCoord.s, texCoord.t, 0, 1 );\n"
    "   gl_FragColor = texture2D( tex, texCoord );\n"
    "}\n";

Shader::Shader() :
    program( 0 ),
    vProgram( vertex_shader ),
    fProgram( fragment_shader )
{

}

Shader::Shader( const std::string &vertexProgram,
                const std::string &fragmentProgram ) :
    program( 0 ),
    vProgram( vertexProgram ),
    fProgram( fragmentProgram )
{

}

Shader::~Shader()
{

}

void Shader::compile()
{
    GLuint v, f;
    const char *p;
    char msg[512];

    p = vProgram.c_str();
    v = glCreateShader( GL_VERTEX_SHADER );
    if ( !v )
    {
        fprintf( stderr, "Couldn't create vertex shader.\n" );
    }
    glShaderSource( v, 1, &p, NULL );
    glCompileShader( v );
    glGetShaderInfoLog( v, sizeof msg, NULL, msg );

    printf( "vertex shader info: %s\n", msg );
    p = fProgram.c_str();
    f = glCreateShader( GL_FRAGMENT_SHADER );
    if ( !f )
    {
        fprintf( stderr, "Couldn't create fragment shader.\n");
    }
    glShaderSource( f, 1, &p, NULL );
    glCompileShader( f );
    glGetShaderInfoLog( f, sizeof msg, NULL, msg );
    printf( "fragment shader info: %s\n", msg );

    program = glCreateProgram();
    glAttachShader( program, v );
    glAttachShader( program, f );
    glBindAttribLocation( program, 0, "position" );

    glLinkProgram( program );
    glGetProgramInfoLog( program, sizeof msg, NULL, msg );
    printf( "info: %s\n", msg );
}

void Shader::activate()
{
    if ( program == 0 )
    {
        compile();
    }

    glUseProgram( program );
}

// TODO store the uniform location in some type of cache.

void Shader::uniform( const GLchar *name, GLfloat f )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform1f( location, f );
}

void Shader::uniform( const GLchar *name, GLfloat f1, GLfloat f2 )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform2f( location, f1, f2 );
}

void Shader::uniform( const GLchar *name,
                      GLfloat f1, GLfloat f2, GLfloat f3 )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform3f( location, f1, f2, f3 );
}

void Shader::uniform( const GLchar *name,
                      GLfloat f1, GLfloat f2, GLfloat f3, GLfloat f4 )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform4f( location, f1, f2, f3, f4 );
}

void Shader::uniform( const GLchar *name, GLint i )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform1i( location, i );
}

void Shader::uniform( const GLchar *name, GLint i1, GLint i2 )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform2i( location, i1, i2 );
}

void Shader::uniform( const GLchar *name,
                      GLint i1, GLint i2, GLint i3 )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform3i( location, i1, i2, i3 );
}

void Shader::uniform( const GLchar *name,
                      GLint i1, GLint i2, GLint i3, GLint i4 )
{
    const GLint location = glGetUniformLocation( program, name );
    glUniform4i( location, i1, i2, i3, i4 );
}
*/
