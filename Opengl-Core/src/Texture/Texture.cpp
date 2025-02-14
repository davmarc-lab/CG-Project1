#include "../../include/Texture/Texture.hpp"

#include "../../include/Graphic.hpp"

#include <iostream>

namespace ogl {

void Texture::onAttach() {
    if (this->m_id)
        return;
    glGenTextures(1, &this->m_id);
}

void Texture::onDetach() {
    if (!this->m_id)
        return;

    // Delete operations

    glDeleteTextures(1, &this->m_id);
    this->m_id = 0;
}

unsigned int Texture::bind() const {
    ASSERT(this->m_id);

    glBindTexture(this->m_params.target, this->m_id);

    return this->m_id;
}

void Texture::unbind() const {}

void Texture::setTexParameteri(const unsigned int &pname, const int &pval) { glTexParameteri(this->m_params.target, pname, pval); }

void Texture::setTexParameterf(const unsigned int &pname, const float &pval) { glTexParameterf(this->m_params.target, pname, pval); }

void Texture::fastCreateTexture2D(void *data) {
    glTexImage2D(this->m_params.target, this->m_params.level, this->m_params.internalFormat, this->m_size.x, this->m_size.y, this->m_params.border,
                 this->m_params.format, this->m_params.dataType, data);
    this->m_created = true;
}

void Texture::generateMipmap() {
    if (this->m_created)
        glGenerateMipmap(this->m_params.target);
    else
     std::cerr << "Error -> Mipmap not generated: this texture (ID => " << this->m_id << ") is not created.\n";
}

void Texture::rescaleTexture() const {
    ASSERT(this->m_id);

    std::cerr << "Not implemented!\n";

}

} // namespace ogl
