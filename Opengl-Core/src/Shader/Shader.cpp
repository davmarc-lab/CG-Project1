#include "../../include/Shader/Shader.hpp"

#include <cassert>

namespace ogl {

void Shader::checkCompileErrors() {
    std::string strType = getShaderTypeString(this->m_type);
    int success;
    char log[1024];

    if (this->m_type != shader::ShaderType::SHADER_TYPE_PROGRAM) {
        glGetShaderiv(this->m_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(this->m_id, 1024, nullptr, log);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << strType << "\n"
                      << log << "\n-- -------------------------------------------------- --\n";
            throw std::exception();
        }
    }
}

void Shader::createShader() {
    const auto rm = ResourceManager::instance();

    this->m_resourceId = rm->addResource(this->m_location, this->m_file);
    assert(rm->getResource(this->m_resourceId).isResourceLoaded());

    assert(this->m_type != shader::SHADER_TYPE_NONE);
    assert(this->m_type != shader::SHADER_TYPE_PROGRAM);

    auto id = glCreateShader(shader::getShaderTypeValue(this->m_type));
    assert(id);

    std::string strCode = rm->getResource(this->m_resourceId).getResourceContent();
    auto length = static_cast<int>(strCode.length());
    const char *code = strCode.c_str();

    glShaderSource(id, 1, &code, &length);
    glCompileShader(id);
    this->m_id = id;
    this->checkCompileErrors();
}

ShaderProgram::ShaderProgram(const std::string &vloc, const std::string &vfile, const std::string &floc, const std::string &ffile) {
    assert(!vloc.empty());
    assert(!vfile.empty());
    assert(!floc.empty());
    assert(!ffile.empty());

    this->m_vert = CreateUnique<Shader>(vloc, vfile, shader::SHADER_TYPE_VERTEX);
    this->m_frag = CreateUnique<Shader>(floc, ffile, shader::SHADER_TYPE_FRAGMENT);
}

ShaderProgram::ShaderProgram(const std::string &vloc, const std::string &vfile, const std::string &floc, const std::string &ffile, const std::string &gloc, const std::string &gfile)
    : ShaderProgram(vloc, vfile, floc, ffile) {
    assert(!gloc.empty());
    assert(!gfile.empty());

    this->m_geom = CreateUnique<Shader>(gloc, gfile, shader::SHADER_TYPE_GEOMETRY);
}

void ShaderProgram::checkLinkingErrors() {
    int success;
    char log[1024];

    glGetProgramiv(this->m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->m_id, 1024, NULL, log);
        std::cout << "ERROR::SHADER_PROGRAM_LINKING_ERROR of type: " << "PROGRAM" << "\n"
                  << log << "\n-- -------------------------------------------------- --\n";
        throw std::exception();
    }
}

void ShaderProgram::createShaderProgram() {
    this->m_vert->createShader();
    this->m_frag->createShader();

    if (this->m_geom != nullptr)
        this->m_geom->createShader();

    this->m_id = glCreateProgram();

    glAttachShader(this->m_id, this->m_vert->getId());
    glAttachShader(this->m_id, this->m_frag->getId());
    if (this->m_geom != nullptr)
        glAttachShader(this->m_id, this->m_geom->getId());

    glLinkProgram(this->m_id);
    this->checkLinkingErrors();

    glDeleteShader(this->m_vert->getId());
    this->m_vert.reset();
    glDeleteShader(this->m_frag->getId());
    this->m_frag.reset();
    if (this->m_geom != nullptr) {
        glDeleteShader(this->m_geom->getId());
        this->m_geom.reset();
    }
}

void ShaderProgram::use() const { glUseProgram(this->m_id); }

void ShaderProgram::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->getId(), name.c_str()), (int)value);
}

void ShaderProgram::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->getId(), name.c_str()), value);
}

void ShaderProgram::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->getId(), name.c_str()), value);
}

void ShaderProgram::setVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(this->getId(), name.c_str()), x, y);
}

void ShaderProgram::setVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(this->getId(), name.c_str()), x, y, z);
}

void ShaderProgram::setVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(this->getId(), name.c_str()), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(this->getId(), name.c_str()), x, y, z, w);
}

void ShaderProgram::setMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->getId(), name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

} // namespace ogl
