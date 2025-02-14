#pragma once

#include "Buffer.hpp"
#include <cstddef>

namespace ogl {
class VertexBuffer : public Buffer {
  public:
    template <typename T> inline void setup(const T *vertices, const size_t &size, const unsigned int &usage) {
        this->bind();
        glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), vertices, usage);
    }

    template <typename T> inline void setupSubData(const T *vertices, const size_t &size, const int &offset) {
        // TODO
    }

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void bind() const override;
    virtual void unbind() const override;

    VertexBuffer() = default;
    ~VertexBuffer() override;
};
} // namespace ogl
