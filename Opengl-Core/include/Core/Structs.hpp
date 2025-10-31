#pragma once

/**
 * @brief Simple data structure to store a RGBA color.
 */
struct Color {
	/// red channel
	float r = 0.f;
	/// green channel
	float g = 0.f;
	/// blue channel
	float b = 0.f;
	/// alpha channel
	float a = 1.f;
};

/**
 * @brief Data structure to create an ogl::Texture using these parameters.
 */
struct TextureParams {
	/// texture target
	unsigned int target = 0;
	/// texture level
	int level = 0;
	/// texture internal data format
	int internalFormat = 0;
	/// texture border
	int border = 0;
	/// texture data format
	unsigned int format = 0;
	/// texture data type
	unsigned int dataType = 0;
};

/**
 * @brief Data structure to create an ogl::RenderBuffer using these parameters.
 */
struct RBConfig {
    /// render buffer format
	unsigned int format = 0;
    /// render buffer attachment
	unsigned int attachment = 0;
    /// render buffer image width
	unsigned int width = 0;
    /// render buffer image height
	unsigned int height = 0;
};

/**
 * @brief Data structure to create an ogl::FrameBuffer using these parameters.
 */
struct FBConfig {
    /// frame buffer operation
	unsigned int operation = 0;
    /// frame buffer width
	unsigned int width = 0;
    /// frame buffer height
	unsigned int height = 0;
};
