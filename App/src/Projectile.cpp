#include "../include/Projectile.hpp"

#include "../include/Factory.hpp"

Projectile::Projectile(const ProjInfo& info, const glm::vec3 pos, const ogl::ShaderProgram &shader, const glm::vec4 &color) : m_info(info) {
    auto e = createProjectile(pos, PROJ_SIZE, {}, color);
    this->setCoordsVector(e->getCoordsVector());
    this->setColorVector(e->getColorVector());
    this->setIndicesVector(e->getIndicesVector());
    this->setScale(e->getScale());
    this->setCollidable(true);

    this->setPosition(pos);
    this->m_startPos = pos;
    this->setRenderCall([&shader, this]() {
        this->setPosition(this->getPosition() + this->m_velocity);
        shader.setMat4("model", this->getModelMatrix());
        this->bindVAO();
        glDrawArrays(GL_TRIANGLE_FAN, 0, this->getCoordsVector().size());
    });
}
