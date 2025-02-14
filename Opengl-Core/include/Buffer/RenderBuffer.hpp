#pragma once

#include "Buffer.hpp"

#include "../Core/Structs.hpp"

namespace ogl {
class RenderBuffer : public Buffer {
  public:
    void rescaleRenderBuffer(const unsigned int &width, const unsigned int &height);

    inline unsigned int getAttachmentType() const { return this->m_config.attachment; }

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void bind() const override;
    virtual void unbind() const override;

    RenderBuffer() = default;
    RenderBuffer(const RBConfig &config) : m_config(config) {}
    ~RenderBuffer() override;

  private:
    RBConfig m_config{};
};
} // namespace ogl
