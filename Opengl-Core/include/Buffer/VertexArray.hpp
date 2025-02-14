#pragma once

#include "Buffer.hpp"

namespace ogl {
class VertexArray : public Buffer {
  public:
    // void linkVertexBuffer();
    void linkAttribFast(const unsigned int &layout, const int &size, const unsigned int &type,
                        const bool &normalize, const int &stride, void *offset) const;

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void bind() const override;
    virtual void unbind() const override;

    VertexArray() = default;
    virtual ~VertexArray() override;
};
} // namespace ogl
