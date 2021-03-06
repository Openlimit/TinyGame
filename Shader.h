#pragma once
#include <string>

#include "OpenGL_Common.h"


// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
    // State
    GLuint ID;
    bool auto_update_VP;
    bool auto_update_CameraPos;

    // Constructor
    Shader():auto_update_VP(false), auto_update_CameraPos(false){ }

    ~Shader()
    {
        glDeleteProgram(this->ID);
    }

    // Sets the current shader as active
    Shader* Use();
    // Compiles the shader from given source code
    void    Compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource = nullptr); // Note: geometry source code is optional 
    // Utility functions
    void SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = false);
    void SetInteger(const GLchar* name, GLint value, GLboolean useShader = false);
    void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = false);
    void SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = false);
    void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
    void SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = false);
    void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
    void SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = false);
    void SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = false);
    void SetIntegerV(const GLchar* name, const GLint* values, int num, GLboolean useShader = false);
    void SetFloatV(const GLchar* name, const GLfloat* values, int num, GLboolean useShader = false);
    void Set2FloatV(const GLchar* name, const GLfloat values[][2], int num, GLboolean useShader = false);
    
private:
    // Checks if compilation or linking failed and if so, print the error logs
    void    checkCompileErrors(GLuint object, std::string type);
};
