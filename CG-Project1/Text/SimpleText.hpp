#include "../Lib.hpp"
#include "../Shader/Shader.hpp"

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
        vec2 position = vec2(0, 0);
        
    public:
        Text(mat4 projection, const int fontSize);

        Text();

        void createVertexArray();

        void initializeTextRender();

        void renderText(Shader shader, string text, float x, float y, float scale, vec4 color);

        void setPosition(vec2 position) { this->position = position; }

        vec2 getPosition() { return this->position; }

        void clear();
};
