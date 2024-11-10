#version 420 core

in VS_OUT {
    vec2 texCoord;
} fs_out;

uniform sampler2D text;
uniform vec3 textColor;

out vec4 fragColor;

void main() {
    fragColor = vec4(textColor, 1.) * vec4(1., 1., 1., texture(text, fs_out.texCoord).r);
}
