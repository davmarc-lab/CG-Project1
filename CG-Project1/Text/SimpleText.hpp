#include "../Lib.hpp"
#include "../Shader/Shader.hpp"
#include "../Color/Color.hpp"

#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H

class Text
{
    private:
        GLuint vao;
        GLuint vbo;
        GLuint texture;
        mat4 projection;
        unsigned int fontSize = 20;

        string text;
        vec2 position = vec2(0, 0);
        Color color = Color(vec4(0));
        
    public:
        Text(mat4 projection, string text, const int fontSize);

        Text();

        void createVertexArray();

        void initializeTextRender();

        void renderText(Shader shader, float x, float y, float scale);

        void setPosition(vec2 position) { this->position = position; }

        vec2 getPosition() { return this->position; }

        void setColor(Color color) { this->color = color; }

        vec4 getColorValues() { return this->color.getColorVector(); }

        void setText(string text) { this->text = text; }

        void appendText(string text) { this->text.append(text); }

        string getText() {return this->text; }

        void clear();
};
