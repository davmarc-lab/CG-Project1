#pragma once

#include "Buffer.hpp"

#include "RenderBuffer.hpp"

#include "../Texture/Texture.hpp"

#include "../Core/Structs.hpp"

namespace ogl {
class FrameBuffer : public Buffer {
  public:
    inline unsigned int getRenderBufferId() const { return this->m_rbo.getId(); }
    void setRenderBuffer(const RBConfig &config);
    void setRenderBuffer(const RenderBuffer &buffer);

    inline unsigned int getColorTexture() const { return this->m_texture.getId(); }
    void setColorTexture(const Texture &texture);

    bool createFrameBuffer();
    void rescaleFrameBuffer(const unsigned int &width, const unsigned int &height);

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void bind() const override;
    virtual void unbind() const override;

    FrameBuffer() = delete;
    FrameBuffer(const FBConfig &config) : m_config(config) {}
    ~FrameBuffer() override;

  private:
    FBConfig m_config{};

    RenderBuffer m_rbo{};
    Texture m_texture{};
};
} // namespace ogl
