#ifndef SHADER_H
#define SHADER_H

/*
#include <glew.h>
#include <string>

class Matrix;

class Shader
{
public:
    Shader();
    Shader( const std::string &vertexProgram,
            const std::string &fragmentProgram );
    ~Shader();

    void compile();
    void activate();

    // Float uniforms
    void uniform( const GLchar *name, GLfloat f );
    void uniform( const GLchar *name, GLfloat f1, GLfloat f2 );
    void uniform( const GLchar *name, GLfloat f1, GLfloat f2, GLfloat f3);
    void uniform( const GLchar *name, GLfloat f1,
                  GLfloat f2, GLfloat f3, GLfloat f4 );

    // Int uniforms
    void uniform( const GLchar *name, GLint i );
    void uniform( const GLchar *name, GLint i1, GLint i2 );
    void uniform( const GLchar *name, GLint i1, GLint i2, GLint i3 );
    void uniform( const GLchar *name, GLint i1, GLint i2, GLint i3, GLint i4 );

    void uniform( const GLchar *name, const Matrix &matrix );

    GLuint program;

private:
    std::string vProgram;
    std::string fProgram;
};
*/

#endif // SHADER_H
