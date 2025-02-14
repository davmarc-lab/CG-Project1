#pragma once

#include "../../include/Core/Structs.hpp"
#include "../../include/Core/Utils.hpp"

namespace ogl {

class Texture {
  public:
    /*
     * Initialize the texture.
     * This operation must be done before using this object.
     */
    void onAttach();

    /*
     * Destroy the texture on the GPU.
     */
    void onDetach();

    /*
     * Bind the current texture id.
     *
     * @returns texture id.
     */
    unsigned int bind() const;

    /*
     * Unbind the current texture id.
     */
    void unbind() const;

    /*
     * Set an `int` parameter for the current texture in bound.
     *
     * @param pname Parameter name.
     * @param pval Parameter `int` value.
     */
    void setTexParameteri(const unsigned int &pname, const int &pval);

    /*
     * Set a `float` parameter of the current texture in bound.
     *
     * @param pname Parameter name.
     * @param pval Parameter `float` value.
     */
    void setTexParameterf(const unsigned int &pname, const float &pval);

    /*
     * Bind the `Texture` id and calls `fastCreatetexture2D`.
     *
     * @param *data Pointer to texture's data.
     */
    inline void createTexture2D(void *data) {
        this->bind();
        this->fastCreateTexture2D(data);
    }

    void fastCreateTexture2D(void *data);

    void generateMipmap();

    inline unsigned int getId() const { return this->m_id; }

    inline unsigned int getWidth() const { return this->m_size.x; }
    inline void setWidth(const unsigned int &width) {
        this->m_size.x = width;
        this->rescaleTexture();
    }

    inline unsigned int getHeight() const { return this->m_size.y; }
    inline void setHeight(const unsigned int &height) {
        this->m_size.y = height;
        this->rescaleTexture();
    }

    inline Pair<unsigned int> getSize() const { return this->m_size; }
    inline void setSize(const Pair<unsigned int> &size) {
        this->m_size = size;
        this->rescaleTexture();
    }

    Texture() = default;

    Texture(const TextureParams &params, const Pair<unsigned int> &size) : m_params(params), m_size(size) {}
    Texture(const TextureParams &params, const unsigned int &width, const unsigned int &height) : m_params(params), m_size({width, height}) {}

    ~Texture() = default;

  private:
    void rescaleTexture() const;

    unsigned int m_id = 0;

    TextureParams m_params{};

    Pair<unsigned int> m_size{};

    bool m_attached = false;
    bool m_created = false;
};

} // namespace ogl
