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
        unsigned int fontSize;
        
    public:
        Text(mat4 projection, const int fontSize);

        Text();

        void createVertexArray();

        void initializeTextRender();

        void renderText(Shader shader, string text, float x, float y, float scale, vec4 color);

        void clear();
};
