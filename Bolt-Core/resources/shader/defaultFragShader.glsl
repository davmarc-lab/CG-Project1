#version 420 core

in vec2 texCoord;
in vec4 vertColor;

in VS_OUT {
    vec4 vertColor;
    vec2 texCoord;
} fs_out;

out vec4 fragColor;

void main() {
    fragColor = fs_out.vertColor;
}
