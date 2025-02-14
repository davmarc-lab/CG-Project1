#pragma once

#include "../Graphic.hpp"

namespace ogl {
class Buffer {
  public:
    inline unsigned int getId() const { return this->m_id; }

    inline virtual void onAttach() {}
    inline virtual void onDetach() {}

    inline virtual void bind() const {}
    inline virtual void unbind() const {}

    Buffer() = default;
    virtual ~Buffer() = default;

  protected:
    unsigned int m_id = 0;
};
} // namespace ogl
