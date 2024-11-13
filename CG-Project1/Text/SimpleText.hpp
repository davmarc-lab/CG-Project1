#include "../Lib.hpp"
#include "../Shader/Shader.hpp"
#include "../Color/Color.hpp"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

/// Single character properties.
struct Character {
    unsigned int TextureID;     // ID handle of the glyph this->texture
    ivec2 Size;                 // Size of glyph
    ivec2 Bearing;              // Offset from baseline to left/top of glyph
    unsigned int Advance;       // Horizontal offset to advance to next glyph
};

/*
 * This class create a text entity using freetype library.
 */
class Text
{
    private:
        GLuint vao;
        GLuint vbo;
        GLuint texture;
        mat4 projection;
        unsigned int fontSize = 20;

        FT_Library library;
        FT_Face fontFace;
        map<GLchar, Character> characters;

        string text;
        vec2 position = vec2(0, 0);
        Color color = Color(vec4(0));

        float totalWidth = 0;
        float totalHeight = 0;

    public:
        // Constructs an objetc with given projection, text and font size.
        Text(mat4 projection, string text, const int fontSize);

        Text();

        // Creates and initialize buffers.
        void createVertexArray();

        // Prepare the text for rendering.
        void initializeTextRender();

        // Renders the text given before.
        void renderText(Shader shader, float x, float y, float scale);

        // Sets the text position.
        void setPosition(vec2 position) { this->position = position; }

        // Retrieves the text position.
        vec2 getPosition() { return this->position; }

        // Retrieves text total width.
        float getTotalWidth() { return this->totalWidth; }

        // Retrieves text total height.
        float getTotalHeight() { return this->totalHeight; }

        // Sets the color of the text.
        void setColor(Color color) { this->color = color; }

        // Retrieves the text color vec4.
        vec4 getColorValues() { return this->color.getColorVector(); }

        // Sets the text.
        void setText(string text) { this->text = text; }

        // Add text at the current text.
        void appendText(string text) { this->text.append(text); }

        // Retrieves the text.
        string getText() {return this->text; }

        void clear();
};
