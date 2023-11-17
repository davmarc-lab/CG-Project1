#include "Shader.hpp"

#include <fstream>
#include <sstream>

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    // check if ifstreams can throw exceptions
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    // tries to read the files provided
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl << e.code() << endl;
    }

    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();

    // compiles the provided shaders
    GLuint vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // initialize shader program
    this->setId(glCreateProgram());
    glAttachShader(this->getId(), vertex);
    glAttachShader(this->getId(), fragment);
    glLinkProgram(this->getId());
    checkCompileErrors(this->getId(), "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(this->getId());
}

void Shader::clearShader()
{
    glDeleteProgram(this->id);
}

void Shader::setBool(const string &name, bool value)
{
    glUniform1i(glGetUniformLocation(this->getId(), name.c_str()), (int)value);
}

void Shader::setInt(const string &name, bool value)
{
    glUniform1i(glGetUniformLocation(this->getId(), name.c_str()), value);
}

void Shader::setFloat(const string &name, bool value)
{
    glUniform1f(glGetUniformLocation(this->getId(), name.c_str()), value);
}

void Shader::setVec2(const string &name, const vec2 &value)
{
    glUniform2fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const string &name, float x, float y)
{
    glUniform2f(glGetUniformLocation(this->getId(), name.c_str()), x, y);
}

void Shader::setVec3(const string &name, const vec3 &value)
{
    glUniform3fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const string &name, float x, float y, float z)
{
    glUniform3f(glGetUniformLocation(this->getId(), name.c_str()), x, y, z);
}

void Shader::setVec4(const string &name, const vec4 &value)
{
    glUniform4fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const string &name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(this->getId(), name.c_str()), x, y, z, w);
}

void Shader::setMat2(const string &name, const mat2 &mat)
{
    glUniformMatrix2fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const string &name, const mat3 &mat)
{
    glUniformMatrix3fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const string &name, const mat4 &mat)
{
    glUniformMatrix4fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
